/*
 * skiplist.h
 * 
 * A simple class template that implements the skip list data struct.
 *
 * The heads and tails are created as sentinel values to make the coding
 * easier.
 * 
 * The underlying data structure is a doubled linked list plus a below pointer.
 *
 * note, there is no duplicate in the list, and insert a value already in the list
 * will not change the list.
 *
 */
 
#include <chrono>
#include <random>

#ifndef SKIP_LIST
#define SKIP_LIST

namespace std {
   template<typename _T>
   class skip_list {
      typedef struct item {
         _T val;
         item *prev;
         item *next;
         item *below;
      } item;
   private:
      _T min_val; // Min value of the heads
      _T max_val; // Max value of the tails

      item heads[4];
      item tails[4];

      bool has_min;
      bool has_max;

      minstd_rand0 rnd;
   public:
      skip_list() {
         // Initialize the random generator.
         unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
         rnd.seed(seed);

         // Record the min and max value.
         min_val = numeric_limits<_T>::min();
         max_val = numeric_limits<_T>::max();
         has_min = false;
         has_max = false;

         // Create the sentinel value of heads and tails in all 4 levels.
         for (int i = 0; i < 4; i++) {
            heads[i].prev = nullptr;
            heads[i].next = tails + i;
            heads[i].val = min_val;
            if (i + 1 < 4) heads[i].below = heads + i + 1;
            else heads[i].below = nullptr;
            tails[i].prev = heads + i;
            tails[i].next = nullptr;
            tails[i].val = max_val;
            if (i + 1 < 4) tails[i].below = tails + i + 1;
            else tails[i].below = nullptr;
         }
      }

      /* For sentinel values, simply mark they are now valid in the skip lists */
      void insert(_T val) {
         if (val == min_val) {
            has_min = true;
         }
         else if (val == max_val) {
            has_max = true;
         }
         else {
            /* Always search first before we insert the value */
            if (!search(val)) {
               item *n = heads;
               item *above = nullptr;

               int levels = 0;
               int level = 3;

               for (int i = 0; i < 3; i++) levels += rnd() & 1; // random number 0 ~ 3

               while (level >= 0) {
                  while (n->val < val) n = n->next;
                  n = n->prev;
                  if (level <= levels) {
                     item *new_node = new item;
                     new_node->val = val;
                     new_node->prev = n;
                     new_node->next = n->next;
                     new_node->next->prev = new_node;
                     n->next = new_node;
                     new_node->below = nullptr;
                     if (above != nullptr) above->below = new_node;
                     above = new_node;
                  }
                  n = n->below;
                  level--;
               }
            }
         }
      }

      bool search(_T val) {
         return _search(val) != nullptr;
      }

      void remove(_T val) {
         item *n = _search(val);

         if (n == heads) {
            has_min = false;
         }
         else if (n == tails) {
            has_max = false;
         }
         else {
            while (n != nullptr) {
               n->prev->next = n->next;
               n->next->prev = n->prev;
               item *below = n->below;
               delete n;
               n = below;
            }
         }
      }
   private:
      item *_search(_T val) {
         if (val == min_val) {
            return has_min ? heads : heads;
         }
         else if (val == max_val) {
            return has_max ? tails : tails;
         }
         else {
            item *node = heads;

            while (node && node->val < val) {
               if (node->next->val <= val) node = node->next; // try to skip first
               else if (node->next->val > val) node = node->below; // go down one level
            }

            return node;
         }

         return nullptr;
      }
   };
};

#endif // SKIP_LIST