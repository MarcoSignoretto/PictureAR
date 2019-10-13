//
// Created by Marco Signoretto on 13/10/2019.
//

#include <string>
#include "gino.h"

using namespace mcv;

mcv::Gino::Gino(const std::vector<const std::string &> &markers, const std::vector<const std::string &> &replacements)
:m_markers{markers},
m_replacements{replacements}
{

}

inline void Gino::init() {

}
