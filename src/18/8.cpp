//jnaranjo Advent of code 18

#include <18/8.h>

#include <vector>

namespace Y18 {
  namespace D8 {

    struct Node {
      std::vector<Node*> children;
      std::vector<uint32> metadata;

      Node() : children(), metadata() {}
    };

    static Node* GenerateGraph(const uint32* buff, std::vector<Node*>* pool,
                               uint32& it, uint32& metadata) {
      Node* n = new Node();
      pool->push_back(n);

      const uint32 num_child = buff[it];
      ++it;
      const uint32 num_metadata = buff[it];
      ++it;
      for (uint32 i = 0; i < num_child; ++i) {
        Node* c = GenerateGraph(buff, pool, it, metadata);
        n->children.push_back(c);
      }
      
      const uint32 fit = it + num_metadata;
      for (uint32 i = it; i < fit; ++i) {
        const uint32 m = buff[i];
        n->metadata.push_back(m);
        metadata += m;
      }
      it += num_metadata;

      return n;
    }

    static uint32 ComputeMetadata(Node* root) {
      uint32 count = 0;

      const std::vector<Node*>& children = root->children;
      const uint32 num_children = (uint32)children.size();

      const std::vector<uint32>& metadata = root->metadata;
      const uint32 num_metadata = (uint32)metadata.size();

      if (num_children == 0) {
        for (uint32 i = 0; i < num_metadata; ++i) {
          count += metadata[i];
        }
      } else {
        for (uint32 i = 0; i < num_metadata; ++i) {
          const uint32 m = metadata[i] - 1;
          if (m < num_children)
            count += ComputeMetadata(children[m]);
        }
      }

      return count;
    }

    void puzzle() {

      std::vector<Node*> pool;
      
      uint32 it = 0;
      uint32 metadata = 0;
      Node* root = GenerateGraph(input, &pool, it, metadata);

      LOG("The first part is: %u", metadata);

      metadata = ComputeMetadata(root);

      LOG("The second part is: %u", metadata);

      const uint32 count_nodes = (uint32)pool.size();
      for (uint32 i = 0; i < count_nodes; ++i) {
        delete pool[i];
      }
    }
  }
}

