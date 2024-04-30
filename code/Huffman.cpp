// Name: Nicole Sawtelle
// github username: nsawtelle
// CU Boulder username: nisa1697

#include "Huffman.h"

freq_info* huff_create_leaf(char symbol, int count) {
  freq_info* leaf = new freq_info;
  leaf->symbol = symbol;
  leaf->count = count;

  leaf->left = NULL;
  leaf->right = NULL;

  leaf->is_leaf = true;
  return leaf;
}

freq_info* huff_combine_nodes(freq_info* left, freq_info* right) {
  freq_info* combined_node = new freq_info;

  combined_node->symbol = '\0';

  combined_node->count = left->count + right->count;

  combined_node->left = left;
  combined_node->right = right;

  combined_node->is_leaf = false;

  return combined_node;
}

void huff_incr_lookup(map<char, int>& lookup, char symbol) {
  auto it = lookup.find(symbol);

  if (it != lookup.end()) {
    it->second++;
  } else {
    lookup[symbol] = 1;
  }
}

void huff_load_queue(const map<char, int>& lookup, tree_queue& q) {
  for (const auto& pair : lookup) {
    freq_info* node = huff_create_leaf(pair.first, pair.second);
    q.push(node);
  }
}

freq_info* huff_build_tree(tree_queue& q) {
  while (q.size() > 1) {
    freq_info* left = q.top();
    q.pop();
    freq_info* right = q.top();
    q.pop();

    freq_info* combined = huff_combine_nodes(left, right);

    q.push(combined);
  }
  return q.top();
}

void huff_build_enc_table_helper(freq_info* node, string code, map<char, string>& enc_table);
map<char, string> huff_build_enc_table(freq_info* root) {
  
  map<char, string> enc_table;
  string code;

  if (root == nullptr)
    return enc_table;

  huff_build_enc_table_helper(root, code, enc_table);
  return enc_table;
}

void huff_build_enc_table_helper(freq_info* node, string code, map<char, string>& enc_table) {
  if (node->is_leaf) {
    enc_table[node->symbol] = code;
    return;
  }

  huff_build_enc_table_helper(node->left, code + LEFT_STR, enc_table);

  huff_build_enc_table_helper(node->right, code + RIGHT_STR, enc_table);
}


string huff_encode(map<char, string> enc_table, string input) {
  string encoded_string;

  for (char c : input) {

    auto it = enc_table.find(c);
    if (it != enc_table.end()) {
      encoded_string += it->second;
    } else {

    }
  }
  return encoded_string;;
}

string huff_decode(freq_info* root, string input) {
  string decoded_string;
  freq_info* current = root;


  for(char bit : input) {

    if (bit == '0') {
      current = current->left;
    } else if (bit == '1') {
      current = current->right;
    }

  }


  if (current->is_leaf) {
    decoded_string += current->symbol;

    current = root;
  }

  if (current != root && current->is_leaf) {
    decoded_string += current->symbol;
  }
  return decoded_string;
}