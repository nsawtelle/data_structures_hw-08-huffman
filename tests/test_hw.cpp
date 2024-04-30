// If you change anything in this file, your changes will be ignored
// in your homework submission.
// Chekout TEST_F functions bellow to learn what is being tested.
#include "../code/Huffman.h"
#include <gtest/gtest.h>

#include <cmath>
#include <ctime>
#include <fstream>
#include <iostream>
#include <math.h>
#include <string>

using namespace std;

class test_Huffman : public ::testing::Test {
protected:
  // This function runs only once before any TEST_F function
  static void SetUpTestCase() {
    std::ofstream outgrade("./total_grade.txt");
    if (outgrade.is_open()) {
      outgrade.clear();
      outgrade << (int)0;
      outgrade.close();
    }
  }

  // This function runs after all TEST_F functions have been executed
  static void TearDownTestCase() {
    std::ofstream outgrade("./total_grade.txt");
    if (outgrade.is_open()) {
      outgrade.clear();
      outgrade << (int)std::ceil(100 * total_grade / max_grade);
      outgrade.close();
      std::cout << "Total Grade is : "
                << (int)std::ceil(100 * total_grade / max_grade) << std::endl;
    }
  }

  void add_points_to_grade(double points) {
    if (!::testing::Test::HasFailure()) {
      total_grade += points;
    }
  }

  // this function runs before every TEST_F function
  void SetUp() override {}

  // this function runs after every TEST_F function
  void TearDown() override {
    std::ofstream outgrade("./total_grade.txt");
    if (outgrade.is_open()) {
      outgrade.clear();
      outgrade << (int)std::ceil(100 * total_grade / max_grade);
      outgrade.close();
    }
  }

  static double total_grade;
  static double max_grade;
};

double test_Huffman::total_grade = 0;
double test_Huffman::max_grade = 195;

// forward declaration of helpers
map<char, int> monaMap();
map<char, string> monaEncTbl();
freq_info* monaTree();
string monaEncoded();
string moonmanEncoded();
void debugTree(freq_info* n, string spaces);
void fullCircuitFile(string fn, string msg, int expected_size_encoded);

TEST_F(test_Huffman, CreateLeafFrequencyNode) {
  freq_info* n = huff_create_leaf('e', 99);
  ASSERT_TRUE(n);
  add_points_to_grade(1);
  ASSERT_EQ(n->symbol, 'e');
  add_points_to_grade(2);
  ASSERT_EQ(n->count, 99);
  add_points_to_grade(2);
  ASSERT_FALSE(n->left);
  add_points_to_grade(1);
  ASSERT_FALSE(n->right);
  add_points_to_grade(1);
  ASSERT_TRUE(n->is_leaf);
  add_points_to_grade(2);
}

TEST_F(test_Huffman, CreateInternalFrequencyNode) {
  freq_info* l(new freq_info);
  l->symbol = 'e';
  l->count = 10;
  l->left = NULL;
  l->right = NULL;
  l->is_leaf = true;
  freq_info* r(new freq_info);
  r->symbol = 'x';
  r->count = 2;
  r->left = NULL;
  r->right = NULL;
  r->is_leaf = true;

  freq_info* comb = huff_combine_nodes(l, r);
  ASSERT_TRUE(comb);
  add_points_to_grade(2);
  ASSERT_EQ(comb->count, 12);
  add_points_to_grade(2);
  ASSERT_EQ(comb->left, l);
  add_points_to_grade(2);
  ASSERT_EQ(comb->right, r);
  add_points_to_grade(2);
  ASSERT_FALSE(comb->is_leaf);
  add_points_to_grade(2);
}

TEST_F(test_Huffman, CountSymbols) {
  // simulate a file with a stringstream, put text in it
  std::stringstream fake_file;
  fake_file << "fake file text fabulous fred";

  map<char, int> lookup;
  char ch;
  while (fake_file >> noskipws >> ch) {
    huff_incr_lookup(lookup, ch);
  }
  ASSERT_EQ(lookup['f'], 4);
  add_points_to_grade(2);
  ASSERT_EQ(lookup['s'], 1);
  add_points_to_grade(2);
  ASSERT_EQ(lookup['!'], 0);
  add_points_to_grade(2);
  ASSERT_EQ(lookup['a'], 2);
  add_points_to_grade(2);
}

TEST_F(test_Huffman, LoadQueue) {
  map<char, int> lookup = monaMap();

  tree_queue q;
  huff_load_queue(lookup, q);

  // q should now have the same number of elements as the lookup
  // table.
  ASSERT_EQ(q.size(), lookup.size());
  add_points_to_grade(2);

  // the items in the queue should be in the order: n, o, a, m
  freq_info* n;
  n = q.top();
  q.pop();
  ASSERT_TRUE(n); // pointer shouldn't be NULL
  ASSERT_EQ(n->symbol, 'n');
  add_points_to_grade(2);
  ASSERT_EQ(n->count, 1);
  add_points_to_grade(2);

  n = q.top();
  q.pop();
  ASSERT_TRUE(n); // pointer shouldn't be NULL
  ASSERT_EQ(n->symbol, 'o');
  add_points_to_grade(2);
  ASSERT_EQ(n->count, 8);
  add_points_to_grade(2);

  n = q.top();
  q.pop();
  ASSERT_TRUE(n); // pointer shouldn't be NULL
  ASSERT_EQ(n->symbol, 'a');
  add_points_to_grade(2);
  ASSERT_EQ(n->count, 10);
  add_points_to_grade(2);

  n = q.top();
  q.pop();
  ASSERT_TRUE(n); // pointer shouldn't be NULL
  ASSERT_EQ(n->symbol, 'm');
  add_points_to_grade(2);
  ASSERT_EQ(n->count, 15);
  add_points_to_grade(2);
}

TEST_F(test_Huffman, BuildTree) {
  // requires load_queue to work
  map<char, int> lookup = monaMap();
  tree_queue q;

  huff_load_queue(lookup, q);
  freq_info* r = huff_build_tree(q);
  ASSERT_TRUE(r);
  ASSERT_EQ(r->count, (1 + 8 + 10 + 15));
  add_points_to_grade(3);
  ASSERT_TRUE(r->left);
  add_points_to_grade(3);
  ASSERT_EQ(r->left->count, 15);
  add_points_to_grade(3);
  ASSERT_EQ(r->left->symbol, 'm');
  add_points_to_grade(3);
  ASSERT_TRUE(r->right);
  add_points_to_grade(3);
  ASSERT_EQ(r->right->count, 19);
  add_points_to_grade(3);
  ASSERT_TRUE(r->right->left);
  add_points_to_grade(3);
  ASSERT_EQ(r->right->left->count, 9);
  add_points_to_grade(3);
  ASSERT_TRUE(r->right->right);
  add_points_to_grade(3);
  ASSERT_EQ(r->right->right->symbol, 'a');
  add_points_to_grade(3);
  ASSERT_TRUE(r->right->left->left);
  add_points_to_grade(3);
  ASSERT_EQ(r->right->left->left->symbol, 'n');
  add_points_to_grade(3);
  ASSERT_TRUE(r->right->left->right);
  add_points_to_grade(3);
  ASSERT_EQ(r->right->left->right->symbol, 'o');
  add_points_to_grade(3);
}

TEST_F(test_Huffman, BuildEncodingTable) {
  freq_info* r = monaTree();

  map<char, string> enc_tbl = huff_build_enc_table(r);
  map<char, string> key = monaEncTbl();
  ASSERT_EQ(key['m'], enc_tbl['m']);
  add_points_to_grade(7);
  ASSERT_EQ(key['a'], enc_tbl['a']);
  add_points_to_grade(7);
  ASSERT_EQ(key['o'], enc_tbl['o']);
  add_points_to_grade(7);
  ASSERT_EQ(key['n'], enc_tbl['n']);
  add_points_to_grade(7);
}

TEST_F(test_Huffman, EncodeString) {

  map<char, string> tbl = monaEncTbl();
  string orig = "mona";
  string encoded = huff_encode(tbl, orig);
  string expected = monaEncoded();
  ASSERT_EQ(encoded, expected);
  add_points_to_grade(20);

  orig = "moonman";
  encoded = huff_encode(tbl, orig);
  expected = moonmanEncoded();
  ASSERT_EQ(encoded, expected);
  add_points_to_grade(20);
}

TEST_F(test_Huffman, DecodeString) {
  freq_info* r = monaTree();

  string encoded = monaEncoded();
  string decoded = huff_decode(r, encoded);
  ASSERT_EQ(decoded, "mona");
  add_points_to_grade(20);

  encoded = moonmanEncoded();
  decoded = huff_decode(r, encoded);
  ASSERT_EQ(decoded, "moonman");
  add_points_to_grade(20);
}

TEST_F(test_Huffman, FullCircuitMicroFile) {
  fullCircuitFile("../tests/micro.txt", "tax does good, all to lists!!", 113);
  fullCircuitFile("../tests/small.txt", "a minute phrase", 68);
  fullCircuitFile("../tests/pkd.txt", "These pretzels are making me thirsty",
                  166);
}

// Helper functions down here. Take a look.

map<char, int> monaMap() {
  map<char, int> lookup;
  lookup['m'] = 15;
  lookup['n'] = 1;
  lookup['o'] = 8;
  lookup['a'] = 10;
  return lookup;
}

map<char, string> monaEncTbl() {
  //    [root]
  //   m    [19]
  //      [9]   a
  //     n   o
  map<char, string> tbl;
  tbl['n'] = RIGHT_STR + LEFT_STR + LEFT_STR;
  tbl['m'] = LEFT_STR;
  tbl['a'] = RIGHT_STR + RIGHT_STR;
  tbl['o'] = RIGHT_STR + LEFT_STR + RIGHT_STR;
  return tbl;
}

freq_info* monaTree() {

  freq_info* nodeM = huff_create_leaf('m', 15);
  freq_info* nodeA = huff_create_leaf('a', 10);
  freq_info* nodeO = huff_create_leaf('o', 8);
  freq_info* nodeN = huff_create_leaf('n', 1);
  freq_info* node9 = huff_combine_nodes(nodeN, nodeO);
  freq_info* node19 = huff_combine_nodes(node9, nodeA);
  freq_info* root = huff_combine_nodes(nodeM, node19);
  return root;
}

string monaEncoded() {
  return LEFT_STR + RIGHT_STR + LEFT_STR + RIGHT_STR + RIGHT_STR + LEFT_STR +
         LEFT_STR + RIGHT_STR + RIGHT_STR;
}

string moonmanEncoded() {
  return LEFT_STR + RIGHT_STR + LEFT_STR + RIGHT_STR + RIGHT_STR + LEFT_STR +
         RIGHT_STR + RIGHT_STR + LEFT_STR + LEFT_STR + LEFT_STR + RIGHT_STR +
         RIGHT_STR + RIGHT_STR + LEFT_STR + LEFT_STR;
}

void debugTree(freq_info* n, string spaces) {
  if (n->is_leaf) {
    cout << spaces << "sym: " << n->symbol << ", count: " << n->count << endl;
  } else {
    cout << spaces << "internal count: " << n->count << endl;
    debugTree(n->left, spaces + "  ");
    debugTree(n->right, spaces + "  ");
  }
}

void fullCircuitFile(string fn, string msg, int expected_size_encoded) {
  // open a file for reading
  fstream fin(fn, fstream::in);

  // create a table that maps symbols with counts
  map<char, int> lookup;
  char ch;

  while (fin >> noskipws >> ch) {
    huff_incr_lookup(lookup, ch);
  }

  // create a priority queue based on this table
  tree_queue q;
  huff_load_queue(lookup, q);

  // create a Huffman codec tree using the priority queue
  freq_info* hufftree = huff_build_tree(q);
  // if you're hung up on this step, consider uncommenting these debugging
  // lines:
  //
  // cout << "root node has total symbol count: " << hufftree->count << endl;
  // debugTree(hufftree, "");

  // create an encoding table based on the Huffman codec tree
  map<char, string> enc_table = huff_build_enc_table(hufftree);

  // encode the text 'tax does good, all to lists!!'
  string encoded = huff_encode(enc_table, msg);

  cout << "Using corpus from '" << fn << "'" << endl;
  cout << "Original: " << msg << endl;
  cout << "Encoded:  " << encoded << endl;

  ASSERT_EQ(encoded.size(), expected_size_encoded);

  // finally decode the encoded string, should get the original back.
  string decoded = huff_decode(hufftree, encoded);
  cout << "Decoded:  " << decoded << endl;
  ASSERT_EQ(decoded, msg);

  float uncompressed_size =
      (float)msg.size() *
      8.0; // mult by 8 because we need to compare bit-length
  float compressed_size = (float)encoded.size();
  cout << "Compression ratio (higher is better): "
       << (uncompressed_size / compressed_size) << endl;

  cout << endl; // blank line for legibilityness.
}
