#include "Save.hpp"
#include <json/json.hpp>
#include <sstream>

JFunc<void, jdouble, jdouble, jdouble> Save::makeBuilder = {};
JFunc<void, jdouble, jdouble> Save::lineTo = {};
JFunc<void, jdouble, jdouble, jdouble> Save::splineTo = {};
JFunc<void, jdouble, jdouble, jdouble> Save::lineToLinearHeading = {};
JFunc<void, jdouble, jdouble, jdouble, jdouble> Save::splineToLinearHeading = {};
JFunc<void, jdouble, jdouble> Save::lineToConstantHeading = {};
JFunc<void, jdouble, jdouble, jdouble> Save::splineToConstantHeading = {};
JFunc<void, jdouble, jdouble, jdouble> Save::lineToSplineHeading = {};
JFunc<void, jdouble, jdouble, jdouble, jdouble> Save::splineToSplineHeading = {};
JFunc<void, jdouble> Save::wait = {};
JFunc<void, jdouble> Save::rotate = {};
JFunc<void, jstring> Save::marker = {};
JFunc<void, jstring> Save::pathErr = {};

void perr(const std::string& str)
{
	jstring s = FuncStat::env->NewStringUTF(str.c_str());
	Save::pathErr.callV(s);
}

bool Save::load(NodeGrid* grid, const std::string& path)
{
	for (int i = 0; i < grid->nodes.count; i++)
	{
		PathNode* node = grid->nodes[i];
		for (NodePart* part : node->parts)
		{
			delete part;
		}
		node->parts.clear();
	}
	grid->nodes.count = 0;
	grid->segs.count = 0;
	FILE* file = fopen(path.c_str(), "r");
	if (file == nullptr)
	{
		perr(("could not open file at" + path).c_str());
		return false;
	}

	fseek(file, 0, SEEK_END);
	int size = ftell(file);
	fseek(file, 0, SEEK_SET);
	char* mem = (char*)calloc(size, 1);
	fread(mem, size, 1, file);
	std::stringstream sstream;
	sstream << mem;
	nlohmann::json json;
	try
	{
		json = nlohmann::json::parse(sstream);
	}
	catch (nlohmann::json::parse_error)
	{
		perr(("could not parse file at " + path).c_str());
		return false;
	}
	delete[] mem;
	try
	{

		int i = 0;
		for (auto jNode : json["nodes"])
		{
			PathNode* node = grid->nodes.add();
			node->pos = {jNode["pos"]["x"], jNode["pos"]["y"]};
			node->layer = jNode["layer"];
			node->rot = jNode["rot"];
			node->heading = jNode["heading"];
			for (auto jPart : jNode["other"])
			{
				if (jPart.contains("text"))
				{
					Marker* marker = new Marker();
					std::string text = jPart["text"];
					marker->text = new char[text.size()];
					strcpy(marker->text, text.c_str());
					node->parts.push_back(marker);
				}
				if (jPart.contains("time"))
				{
					Delay* delay = new Delay();
					delay->time = jPart["time"];
					node->parts.push_back(delay);
				}
				if (jPart.contains("angle"))
				{
					Turn* turn = new Turn();
					turn->angle = jPart["angle"];
					node->parts.push_back(turn);
				}
			}
			i++;
		}
		for (auto jNode : json["segs"])
		{
			PathSegment* seg = grid->segs.add();
			seg->startNode = jNode["startNode"];
			seg->endNode = jNode["endNode"];
			seg->layer = jNode["layer"];
			seg->headingMode = jNode["heading"];
			seg->pathType = jNode["path"];
			seg->recognitionId = jNode["recognitionId"];
			i++;
		}
	}
	catch (nlohmann::json::type_error e)
	{
		perr(e.what());
		return false;
	}
	return true;
}

bool Save::exp(NodeGrid* grid)
{
	uint8_t* segUsage = new uint8_t[grid->nodes.count];
	memset(segUsage, 0, grid->nodes.count);
	for (int i = 0; i < grid->segs.count; i++)
	{
		PathSegment* s = grid->segs.get(i);
		if (s->recognitionId == grid->recognitionId || s->recognitionId == -1)
		{
			segUsage[s->startNode] |= 1;
			segUsage[s->endNode] |= 2;
		}
	}
	int startInd = -1;
	bool emptyNodes = false;
	for (int j = 0; j < grid->nodes.count; j++)
	{
		if (segUsage[j] == 1)
		{
			if (startInd == -1)
			{
				startInd = j;
			}
			else
			{
				perr((std::string("path has multiple start nodes | recognition: ") +
					  std::to_string(grid->recognitionId)));
				return false;
			}
		}
		if (segUsage[j] == 0)
		{
			emptyNodes = true;
		}
	}
	if (emptyNodes)
	{
		print("warning: path has unused nodes");
	}
	std::vector<int> segments;
	int targetInd = startInd;
	bool foundNode = true;
	while (foundNode)
	{
		foundNode = false;
		int foundInd = 0;
		for (int i = 0; i < grid->segs.count; i++)
		{
			PathSegment* seg = grid->segs.get(i);
			if (seg->recognitionId == grid->recognitionId || seg->recognitionId == -1)
			{
				if (seg->startNode == targetInd)
				{
					if (foundNode)
					{
						perr((std::string("fork found at node ") + std::to_string((int)seg->startNode) +
							  " | recognition: " + std::to_string(grid->recognitionId))
								 .c_str());
						return false;
					}

					for (int j = 0; j < segments.size(); j++)
					{
						PathSegment* seg2 = grid->segs[j];
						if (seg->endNode == seg2->endNode)
						{
							perr((std::string("found path loop at segs ") + std::to_string(j) + ", " +
								  std::to_string(i) + " | recognition: " + std::to_string(grid->recognitionId))
									 .c_str());
							return false;
						}
					}

					foundNode = true;
					foundInd = seg->endNode;
					segments.push_back(i);
				}
			}
		}
		targetInd = foundInd;
	}

	float prevHeading;
	glm::vec2 prevPos;

	PathNode* startNode = grid->nodes.get(startInd);
	prevPos = startNode->pos;
	makeBuilder.callV(startNode->pos.x, startNode->pos.y, -(startNode->rot - 90));

	for (int i = 0; i < segments.size(); i++)
	{
		PathSegment* seg = grid->segs.get(segments[i]);
		PathNode* node = grid->nodes.get(seg->endNode);

		node->rot = -(node->rot - 90);
		node->heading = -(node->heading - 90);

		bool ins = false;
		bool constantHeading = false;
		switch (seg->headingMode)
		{
		case 0:
			if (seg->pathType == 0)
			{
				splineTo.callV(node->pos.x, node->pos.y, node->heading);
			}
			else
			{
				lineTo.callV(node->pos.x, node->pos.y);
			}
			break;
		case 1:
			if (seg->pathType == 0)
			{
				splineToLinearHeading.callV(node->pos.x, node->pos.y, node->heading, node->rot);
			}
			else
			{
				lineToLinearHeading.callV(node->pos.x, node->pos.y, node->rot);
			}
			break;
		case 2:
			if (seg->pathType == 0)
			{
				splineToConstantHeading.callV(node->pos.x, node->pos.y, node->heading);
			}
			else
			{
				lineToConstantHeading.callV(node->pos.x, node->pos.y);
			}
			break;
		case 3:
			if (seg->pathType == 0)
			{
				splineToConstantHeading.callV(node->pos.x, node->pos.y, node->heading);
			}
			else
			{
				lineToConstantHeading.callV(node->pos.x, node->pos.y);
			}
			break;
		}
		if (node->pos != prevPos)
		{
			if (!constantHeading)
			{
				prevHeading = node->rot;
			}
			for (NodePart* part : node->parts)
			{
				switch (part->getId())
				{
				case NodePartTurn: {
					Turn* turn = (Turn*)part;
					float angle = -(node->heading - prevHeading);
					if (abs(angle) > 180)
					{
						angle -= 360 * (angle > 0 ? 1 : -1);
					}
					rotate.callV(angle);
					break;
				}
				case NodePartDelay: {
					Delay* delay = (Delay*)part;
					wait.callV(delay->time);
					break;
				}
				case NodePartMarker: {
					jstring str = FuncStat::env->NewStringUTF(((Marker*)part)->text);
					marker.callV(str);
					FuncStat::env->ReleaseStringUTFChars(str, FuncStat::env->GetStringUTFChars(str, nullptr));
					break;
				}
				}
			}
		}
		prevPos = node->pos;
		prevHeading = node->rot;
	}
	return true;
}
