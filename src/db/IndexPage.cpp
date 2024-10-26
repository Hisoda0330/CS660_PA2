#include <db/IndexPage.hpp>
#include <stdexcept>
#include <algorithm>

using namespace db;

IndexPage::IndexPage(Page &page) {
  // TODO pa2: implement
  capacity = 340;
  header = (IndexPageHeader *)(page.data() );
  keys = (int *)(page.data() + sizeof(IndexPageHeader));
  children = (size_t *)(page.data() + sizeof(IndexPageHeader) + capacity * sizeof(int));
}


bool IndexPage::insert(int key, size_t child) {
  // TODO pa2: implement
  children[header->size] = child;
  vec_keys.push_back(key);
  header->size++;
  std::sort(vec_keys.begin(), vec_keys.end(), [](const int &key1, const int &key2) { return key1 < key2; });
  std::vector<int> vec_keys2 ;
  for (int i = 0;i < vec_keys.size();i++)
  {
    vec_keys2.push_back(vec_keys[i]);
  }
 
  for (int i = 0; i < vec_keys.size(); i++)
  {
    keys[i] = vec_keys2.front();
    vec_keys2.erase(vec_keys2.begin());
  }
  if (header->size == capacity)
  {
    return true;
  }
  return false;
  
}

int IndexPage::split(IndexPage &new_page) {
  // TODO pa2: implement
  for (int i = header->size - header->size / 2 + 1; i < header->size; i++)
  {
    new_page.insert(keys[i], children[i]);
  }
  header->size = header->size / 2;
 
  return 0;
}

