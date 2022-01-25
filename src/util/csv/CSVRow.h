#pragma once

#include <iostream>
#include <sstream>
#include <vector>
#include <string>

namespace util::csv {
  class CSVRow {
    public:
      std::string operator[](std::size_t index) const {
        std::string str = std::string(&m_line[m_data[index] + 1], m_data[index + 1] - (m_data[index] + 1));
        str.erase(str.find_last_not_of(" \n\r\t")+1);
        return str;
      }

      std::size_t size() const {
        return m_data.size() - 1;
      }

      void readNextRow(std::istream& str) {
        std::getline(str, m_line);

        m_data.clear();
        m_data.emplace_back(-1);
        std::string::size_type pos = 0;
        while((pos = m_line.find(',', pos)) != std::string::npos) {
          m_data.emplace_back(pos);
          ++pos;
        }
        // Check for trailing comma with no data after it
        pos = m_line.size();
        m_data.emplace_back(pos);
      }

      std::string raw() {
        return this->m_line;
      }

    private:
      std::string m_line;
      std::vector<int> m_data;
  };

  std::istream& operator>>(std::istream& str, CSVRow& data) {
    data.readNextRow(str);
    return str;
  }

  std::ostream& operator<<(std::ostream& os, CSVRow& data) {
    os << data.raw();
    return os;
  }
}
