#include <db/LeafPage.hpp>
#include <stdexcept>
#include <algorithm>

using namespace db;

LeafPage::LeafPage(Page &page, const TupleDesc &td, size_t key_index) : td(td), key_index(key_index) {
  // TODO pa2: implement
  capacity = 53;
  header = (LeafPageHeader*)(page.data());
  data = page.data() + sizeof(LeafPageHeader);
  header->next_leaf = 0;
  header->size = 0;
}

bool LeafPage::insertTuple(const Tuple &t) {
  // TODO pa2: implement
  header->size++;
  auto itor = std::find_if(vec_tuples.begin(), vec_tuples.end(), [t](const Tuple &t1) {
        int index = 0;
        if (t.field_type(0) == type_t::INT)
        {
          index = 0;
        }
        else if (t.field_type(1) == type_t::INT) {
          index = 1;
        }
        else if (t.field_type(2) == type_t::INT) {
          index = 2;
        }
        if (t1.get_field(index) == t.get_field(index))
          return true;
      });
  if (itor != vec_tuples.end())
  {
    vec_tuples.erase(itor);
    header->size--;
  }
  vec_tuples.push_back(t);
  std::sort(vec_tuples.begin(), vec_tuples.end(), [](const Tuple &t1, const Tuple &t2) {
    int index = 0;
    if (t1.field_type(0) == type_t::INT) {
      index = 0;
    } else if (t1.field_type(1) == type_t::INT) {
      index = 1;
    } else if (t1.field_type(2) == type_t::INT) {
      index = 2;
    }
    return t1.get_field(index) < t2.get_field(index);
  });
  for (size_t i = 0;i < vec_tuples.size();i++) {
    uint8_t *slotData = data + i * td.length();
    td.serialize(slotData, vec_tuples[i]);
  }
  if (header->size == capacity ) {
    return true;
  }
  return false;
}

int LeafPage::split(LeafPage &new_page) {
  int start = header->size - header->size / 2 - 1;
  for (size_t i = start; i < header->size; i++) {
    new_page.insertTuple(vec_tuples[i]);
  }
  for (size_t i = start; i < header->size; i++) {
   vec_tuples.erase(vec_tuples.begin());
  }
  header->size = header->size / 2;
  new_page.header->next_leaf = header->next_leaf;
  return 0;
}

Tuple LeafPage::getTuple(size_t slot) const {
  // TODO pa2: implement
  uint8_t *slotData = data + slot * td.length();
  return td.deserialize(slotData);
}
