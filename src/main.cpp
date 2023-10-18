#include <iostream>
#include <vector>
#include <sstream>
#include <fstream>
#include <string>
#include <cstring>
#include <cassert>
#include <map>
#include <FastRoute.h>

using namespace FastRoute;
using namespace std;

struct NetInfo {
  char* name;
  int id;
  vector<PIN> pins;
};

const int MAX_PINS = 1000;
int g_numNets = 0;
int g_numAdjustments = 0;
vector<NetInfo*> g_netInfos;
map<string, int> g_netNameToId;

vector<int> g_vCapacities;
vector<int> g_hCapacities;
vector<int> g_minWidths;
vector<int> g_minSpacings;
vector<int> g_minViaSpacings;

void readGrid(std::ifstream& stream, FT* ft) {
  string line;
  getline(stream, line);
  int gridx;
  int gridy;
  int layers;
  stringstream gridStream(line);
  string token;
  gridStream >> token;
  assert(token == "grid");
  gridStream >> gridx >> gridy >> layers;
  ft->setGridsAndLayers(gridx, gridy, layers);
}

void readVCap(std::ifstream& stream, FT* ft) {
  g_vCapacities.clear();
  string line;
  getline(stream, line);
  string token;
  stringstream vcapStream(line);
  vcapStream >> token;
  assert(token == "vertical");
  vcapStream >> token;
  assert(token == "capacity");
  int vcapValue;
  while (vcapStream >> vcapValue) {
    g_vCapacities.push_back(vcapValue);
  }
}

void readHCap(std::ifstream& stream, FT* ft) {
  g_hCapacities.clear();
  string line;
  getline(stream, line);
  string token;
  stringstream hcapStream(line);
  hcapStream >> token;
  assert(token == "horizontal");
  hcapStream >> token;
  assert(token == "capacity");
  int hcapValue;
  while (hcapStream >> hcapValue) {
    g_hCapacities.push_back(hcapValue);
  }
}

void readMinWidth(ifstream& stream, FT* ft) {
  g_minWidths.clear();
  string line;
  getline(stream, line);
  string token;
  stringstream minwStream(line);
  minwStream >> token;
  assert(token == "minimum");
  minwStream >> token;
  assert(token == "width");
  int minwValue;
  while (minwStream >> minwValue) {
    g_minWidths.push_back(minwValue);
  }
}

void readMinSpacing(ifstream& stream, FT* ft) {
  g_minSpacings.clear();
  string line;
  getline(stream, line);
  string token;
  stringstream minsStream(line);
  minsStream >> token;
  assert(token == "minimum");
  minsStream >> token;
  assert(token == "spacing");
  int minsValue;
  while (minsStream >> minsValue) {
    g_minSpacings.push_back(minsValue);
  }
}

void readViaSpacing(ifstream& stream, FT* ft) {
  g_minViaSpacings.clear();
  string line;
  getline(stream, line);
  string token;
  stringstream minvStream(line);
  minvStream >> token;
  assert(token == "via");
  minvStream >> token;
  assert(token == "spacing");
  int minvValue;
  while (minvStream >> minvValue) {
    g_minViaSpacings.push_back(minvValue);
  }
}

void readTileInfo(ifstream& stream, FT* ft) {
  string line;
  getline(stream, line);
  stringstream tileInfoStream(line);
  int llx;
  int lly;
  int xsize;
  int ysize;
  tileInfoStream >> llx >> lly >> xsize >> ysize;
  ft->setLowerLeft(llx, lly);
  ft->setTileSize(xsize, ysize);
}

void setModifiedCapacities(FT* ft) {
  for (int i = 0; i < g_vCapacities.size(); i++) {
    int layer = i + 1;
    int cap = g_vCapacities[i];
    int w = g_minWidths[i];
    int s = g_minSpacings[i];
    int effective_cap = cap / (w + s);

    ft->addVCapacity(effective_cap, layer);
    ft->addMinSpacing(0, layer);
    ft->addMinWidth(1, layer);
  }
  for (int i = 0; i < g_hCapacities.size(); i++) {
    int layer = i + 1;
    int cap = g_hCapacities[i];
    int w = g_minWidths[i];
    int s = g_minSpacings[i];
    int effective_cap = cap / (w + s);

    ft->addHCapacity(effective_cap, layer);
    ft->addMinSpacing(0, layer);
    ft->addMinWidth(1, layer);
  }
  for (int i = 0; i < g_minViaSpacings.size(); i++) {
    int layer = i + 1;
    ft->addViaSpacing(1, layer);
  }
}

bool isWhitespace(const string& line) {
    for (char c : line) {
        if (!std::isspace(c)) {
            return false;
        }
    }
    return true;
}

void readNumNets(ifstream& stream, FT* ft) {
  string line;
  do { 
    getline(stream, line);
  } while (isWhitespace(line));

  cout << "NOSPACE: " << line << endl;

  stringstream numNetsStream(line);
  string token;
  numNetsStream >> token;
  assert(token == "num");
  numNetsStream >> token;
  assert(token == "net");
  numNetsStream >> g_numNets;
  ft->setNumberNets(g_numNets);
  cout << "NUMNETS: " << g_numNets << endl;
}

void readPin(ifstream& stream, NetInfo* netInfo) {
  string line;
  getline(stream, line);
  stringstream pinStream(line);
  PIN pin;
  pinStream >> pin.x >> pin.y >> pin.layer;
  netInfo->pins.push_back(pin);
}

void readNet(ifstream& stream, FT* ft) {
  string line;
  getline(stream, line);
  stringstream netInfoStream(line);
  NetInfo* netInfo = new NetInfo;
  string netName;
  netInfoStream >> netName;
  netInfo->name = new char[netName.size() + 1];
  strcpy(netInfo->name, netName.c_str());

  netInfoStream >> netInfo->id;
  int numPins;
  netInfoStream >> numPins;
  for (int i = 0; i < numPins; i++) {
    readPin(stream, netInfo);
  }

  g_netInfos.push_back(netInfo);
  if (numPins <= MAX_PINS) {
    ft->addNet(netInfo->name, netInfo->id, numPins, 1, netInfo->pins.data());
    g_netNameToId[netInfo->name] = netInfo->id;
  }
}

void readNumAdjustments(ifstream& stream, FT* ft) {
  string line;
  do { 
    getline(stream, line);
  } while (isWhitespace(line));
  stringstream numAdjustmentsStream(line);
  numAdjustmentsStream >> g_numAdjustments;
  ft->setNumAdjustments(g_numAdjustments);
  cout << "ADJUSTMENTS: " << g_numAdjustments << endl;
}

void readAdjustment(ifstream& stream, FT* ft) {
  string line;
  getline(stream, line);
  stringstream adjustmentStream(line);
  int x1;
  int y1;
  int l1;
  int x2;
  int y2;
  int l2;
  int reducedCap;
  adjustmentStream >> x1 >> y1 >> l1 >> x2 >> y2 >> l2 >> reducedCap;
  ft->addAdjustment(x1, y1, l1, x2, y2, l2, reducedCap / 2);
}

void printNet(ofstream& stream, const NET& net) {
  stream << net.name << " " << g_netNameToId[net.name] << " " << net.route.size() << std::endl;
  for (const ROUTE& rt : net.route) {
    stream << "(" << rt.initX << "," << rt.initY << "," << rt.initLayer << ")-(" << rt.finalX << "," << rt.finalY << "," << rt.finalLayer << ")" << std::endl;
  }
}

int main(int argc, char* argv[]) {
  if (argc != 3) {
    std::cout << "FastRoute4API <infile> <outfile>" << std::endl;
    exit(0);
  }
  ios::sync_with_stdio(false);

  std::ifstream infile(argv[1]);

  FT* ft = new FT;

  readGrid(infile, ft);
  readVCap(infile, ft);
  readHCap(infile, ft);
  readMinWidth(infile, ft);
  readMinSpacing(infile, ft);
  readViaSpacing(infile, ft);
  readTileInfo(infile, ft);
  setModifiedCapacities(ft);
  readNumNets(infile, ft);
  ft->setMaxNetDegree(MAX_PINS);
  for (int i = 0; i < g_numNets; i++) {
    readNet(infile, ft);
  }
  ft->initEdges();
  readNumAdjustments(infile, ft);
  for (int i = 0; i < g_numAdjustments; i++) {
    readAdjustment(infile, ft);
  }
  ft->initAuxVar();
  std::vector<NET> nets;
  ft->run(nets);

  std::ofstream outfile(argv[2]);

  for (const NET& net : nets) {
    printNet(outfile, net);
    outfile << "!" << endl;
  }
  return 0;
}
