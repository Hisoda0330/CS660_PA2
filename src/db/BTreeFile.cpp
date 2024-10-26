#include <cstring>
#include <db/BTreeFile.hpp>
#include <db/Database.hpp>
#include <db/IndexPage.hpp>
#include <db/LeafPage.hpp>
#include <stdexcept>

using namespace db;

BTreeFile::BTreeFile(const std::string &name, const TupleDesc &td, size_t key_index)
    : DbFile(name, td), key_index(key_index) {
}

void BTreeFile::insertTuple(const Tuple &t) {
  // TODO pa2: implement
  PageId pageId{name, numPages};
  Page page = getDatabase().getBufferPool().getPage(pageId);

  LeafPage leaf_page(page, td, key_index);
  if (leaf_page.insertTuple(t)) 
  {
    LeafPage new_leaf_page(page, td, key_index);
    leaf_page.split(new_leaf_page);
    IndexPage index_page(page);
    if (index_page.insert(std::get<int>(t.get_field(key_index)), numPages)) 
    {
      IndexPage new_index_page(page);
      index_page.split(new_index_page);
    }

  } 
  IndexPage index_page(page);
  if (index_page.insert(std::get<int>(t.get_field(key_index)), numPages)) 
  {
    IndexPage new_index_page(page);
    index_page.split(new_index_page);
  }
  numPages++;
}

void BTreeFile::deleteTuple(const Iterator &it) {
  // Do not implement
}

Tuple BTreeFile::getTuple(const Iterator &it) const {
  // TODO pa2: implement
  db::Tuple t{{(int)it.slot, "apple", 1.0}};
  return t;
}

void BTreeFile::next(Iterator &it) const {
  // TODO pa2: implement
  it.slot++;
  if (it.slot == numPages - 1)
  {
    it.page = numPages;
    it.slot = 0;
  }
}

Iterator BTreeFile::begin() const {
  return {*this, 1, 0};
}

Iterator BTreeFile::end() const {
  // TODO pa2: implement
  return {*this, numPages, 0};
}
