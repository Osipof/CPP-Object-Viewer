#include "../Mediator/object_model.h"

void ObjectModel::ParsingVertex(std::vector<PartObject> &models,
                                std::string &line,
                                std::size_t &position_old_vertexes) {
  if (models.back().facets.size()) {
    models.push_back(PartObject());
  }
  position_old_vertexes += 1;
  int count_vertex{};
    char *token = std::strtok(line.data() + 1, " ");
    while (token) {
        ++count_vertex;
        models.back().vertexes.push_back(std::stod(token));
        token = strtok(nullptr, " ");
    }
  if (count_vertex != 3) {
    throw std::invalid_argument("Invalid file .obj");
  }
}

void ObjectModel::ParsingFacet(std::vector<PartObject> &models,
                               std::string &line,
                               std::size_t &position_old_vertexes) {
  if (models.empty() || models.back().vertexes.empty()) {
    throw std::invalid_argument("Invalid file .obj");
  }
  char *token = std::strtok(line.data() + 1, " ");
  std::vector<unsigned> face{};
  while (token) {
      if (*token == '\r') {
          break;
      }
    int tmp = std::stoi(token);
    if (tmp < 0) {
      face.push_back(position_old_vertexes + tmp);
    } else {
      face.push_back(tmp - 1);
    }
    token = strtok(nullptr, " ");
  }
  models.back().facets.push_back(face);
}

void ObjectModel::OpenObject(std::string line) {
  std::ifstream file_in(line);
  std::vector<PartObject> models = {PartObject()};
  std::size_t position_old_vertexes{};
  while (std::getline(file_in, line)) {
    if (line.length() < 2) {
      continue;
    }
    if (line[0] == 'v' && line[1] == ' ') {
      ParsingVertex(models, line, position_old_vertexes);
    } else if (line[0] == 'f') {
      ParsingFacet(models, line, position_old_vertexes);
    }
  }
  for (auto &model_parse : models) {
    model.vertexes.insert(model.vertexes.end(), model_parse.vertexes.begin(),
                          model_parse.vertexes.end());
    model.facets.insert(model.facets.end(), model_parse.facets.begin(),
                        model_parse.facets.end());
  }
  RelocateOnStartPosition();
}
