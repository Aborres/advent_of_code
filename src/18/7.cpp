//jnaranjo Advent of code 18

#include <18/7.h>

#include <utils.h>
#include <vector>
#include <deque>
#include <string>
#include <algorithm>
#include <functional>

namespace Y18 {
  namespace D7 {

    struct Node {
      bool ready;
      char id;
      std::vector<Node*> parent;
      std::vector<Node*> children;

      Node() : ready(false), id(0), parent(), children() {}
    };
    
    static Node* FindNode(char id, const std::vector<Node*>& pool) {
      const uint32 count = (uint32)pool.size();
      for (uint32 i = 0; i < count; ++i) {
        if (pool[i]->id == id)
          return pool[i];
      }
      return nullptr;
    }

    static bool IsReady(Node* n) {
      const uint32 count = (uint32)n->parent.size();
      for (uint32 i = 0; i < count; ++i) {
        if (!n->parent[i]->ready) {
          return false;
        }
      }
      return true;
    }

    typedef bool(*SortingLambda)(Node*, Node*);

    static void ProcessChildren(std::deque<Node*>* ready_pool, 
                                const std::vector<Node*>& children,
                                const SortingLambda& sorting_lambda) {

      const uint32 n_children = (uint32)children.size();
      for (uint32 i = 0; i < n_children; ++i) {
        Node* c = children[i];
        if (IsReady(c)) {
          auto it = std::find(ready_pool->begin(), ready_pool->end(), c);
          if (it == ready_pool->end())
            ready_pool->push_back(c);
        }
      }
      std::sort(ready_pool->begin(), ready_pool->end(), sorting_lambda);
    }

    struct Worker {

      bool isReady() {
        --timer;
        return timer == 0;
      }

      void update(std::deque<Node*>* ready_pool) {
        if (!working) {
          if (ready_pool->size() > 0) {
            n = (*ready_pool)[0];
            ready_pool->pop_front();

            const uint32 t = n->id - 'A' + 1 + 60;
            timer = t;
            working = true;
          }
        }
      }

      void lateUpdate(std::deque<Node*>* ready_pool, const SortingLambda& sorting_lambda) {
        if (working && isReady()) {
          n->ready = true;

          ProcessChildren(ready_pool, n->children, sorting_lambda);
          working = false;
        }
      }

      bool working;
      uint32 timer;
      Node* n;

      Worker() : working(false), timer(0), n(nullptr) {}
    };


    void puzzle() {

      std::vector<Node*> pool;
      
      //Generate graph
      const uint32 count = COUNT_ARR(input);
      for (uint32 i = 0; i < count; ++i) {
        char parent_id, child_id;
        sscanf(input[i], "Step %c must be finished before step %c can begin.", &parent_id, &child_id);
        Node* parent = FindNode(parent_id, pool);
        Node* child  = FindNode(child_id, pool);

        if (!child) {
          child = new Node();
          child->id = child_id;
          pool.push_back(child);
        }

        if (!parent) {
          parent = new Node();
          parent->id = parent_id;
          pool.push_back(parent);
        }
        child->parent.push_back(parent);
        parent->children.push_back(child);
      }

      SortingLambda sorting_lambda = [](Node* left, Node* right) {
          return left->id < right->id;
      };

      const uint32 pool_size = (uint32)pool.size();

      for (uint32 i = 0; i < pool_size; ++i) {
        std::vector<Node*>* c = &pool[i]->children;
        std::sort(c->begin(), c->end(), sorting_lambda);

        std::vector<Node*>* p = &pool[i]->parent;
        std::sort(p->begin(), p->end(), sorting_lambda);
      }

      //Find all possible roots
      std::vector<Node*> roots;
      for (uint32 i = 0; i < pool_size; ++i) {
        Node* n = pool[i];

        if (n->parent.size() == 0) {
          roots.push_back(n);
          n->ready = true;
        }
      }

      //First exercise
      std::string final_string;
      std::deque<Node*> ready_pool(roots.begin(), roots.end());

      while (ready_pool.size() > 0) {
        Node* n = ready_pool[0]; 
        ready_pool.pop_front();
        final_string += n->id;
        n->ready = true;
        ProcessChildren(&ready_pool, n->children, sorting_lambda);
      }

      LOG("The final string is: %s", final_string.c_str());

      //Second exercise
      for (uint32 i = 0; i < pool_size; ++i) {
        pool[i]->ready = false;
      }

      ready_pool.assign(roots.begin(), roots.end());

      const uint32 total_workers = 5;
      Worker workers[total_workers];

      uint32 total_time = 0;
      bool workers_working = false;
      while (ready_pool.size() > 0 || workers_working) {

        workers_working = false;

        for (uint32 i = 0; i < total_workers; ++i) {
          Worker* w = &workers[i];
          w->update(&ready_pool);

          if (!workers_working && w->working)
            workers_working = true;
        }

        if (workers_working) {
          ++total_time;
          for (uint32 i = 0; i < total_workers; ++i) {
            Worker* w = &workers[i];
            w->lateUpdate(&ready_pool, sorting_lambda);
          }
        }
      }

      LOG("Total time: %d", total_time);

      for (uint32 i = 0; i < pool_size; ++i) {
        delete pool[i];
      }
    }
  }
}