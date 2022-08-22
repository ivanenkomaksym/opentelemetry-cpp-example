#include "opentelemetry/sdk/version/version.h"
#include "opentelemetry/trace/provider.h"
#include "opentelemetry/logs/provider.h"

#include <iostream>

#include <algorithm>
#include <cassert>
#include <iostream>
#include <random>
#include <vector>
#include <unordered_set>

namespace logs  = opentelemetry::logs;
namespace trace = opentelemetry::trace;
namespace nostd = opentelemetry::nostd;

namespace
{
    nostd::shared_ptr<trace::Tracer> get_tracer()
    {
      auto provider = trace::Provider::GetTracerProvider();
      return provider->GetTracer("hashset_tracer", OPENTELEMETRY_SDK_VERSION);
    }
    
    nostd::shared_ptr<logs::Logger> get_logger()
    {
      auto provider = logs::Provider::GetLoggerProvider();
      return provider->GetLogger("hashset_logger", "", "hashset");
    }
    
    void log(std::string msg)
    {
      std::cout << "Logging..." << std::endl;

      auto span        = get_tracer()->StartSpan("log span");
      auto scoped_span = trace::Scope(get_tracer()->StartSpan("hash_set"));
      auto ctx         = span->GetContext();
      auto logger      = get_logger();
      logger->Log(opentelemetry::logs::Severity::kDebug,
                  msg,
                  {},
                  ctx.trace_id(),
                  ctx.span_id(),
                  ctx.trace_flags(),
                  opentelemetry::common::SystemTimestamp(std::chrono::system_clock::now()));
    }
    
    struct ListNode
    {
        ListNode(int value):
            val{value}
            {}
            
        int val;
        ListNode *next = nullptr;
    };

    class HashSet {
    public:
        HashSet() {
            
        }
        
        bool add(int key) {
            auto scoped_span = trace::Scope(get_tracer()->StartSpan("HashSet::add"));
  
            if (mySet == nullptr)
            {
                mySet = new ListNode(key);
                return true;
            }
            
            auto it = mySet;
            auto last = it;
            while (it != nullptr)
            {
                if (it->val == key)
                    return false;

                last = it;
                it = it->next;
            }    
            
            last->next = new ListNode(key);
            return true;
        }
        
        bool remove(int key) {
            auto scoped_span = trace::Scope(get_tracer()->StartSpan("HashSet::remove"));
            
            if (mySet == nullptr)
                return false;
            
            ListNode *it = nullptr;
            auto next = mySet;
            while (next != nullptr)
            {
                if (next == nullptr)
                    return false;
                
                if (next->val == key)
                {
                    if (it == nullptr)
                    {
                        mySet = next->next;
                        delete next;
                        next = 0;
                        return true;
                    }
                    else
                    {
                        it->next = next->next;
                        delete next;
                        next = 0;
                        return true;
                    }
                }
                else
                {
                    it = next;
                    next = next->next;
                }
            }
            
            return false;
        }
        
        bool contains(int key) {
            auto scoped_span = trace::Scope(get_tracer()->StartSpan("HashSet::contains"));
            
            auto it = mySet;
            while (it != nullptr)
            {
                if (it->val == key)
                    return true;

                it = it->next;
            }
            
            return false;
        }
        
    private:
        ListNode *mySet = nullptr;
    };

}

namespace ranges
{
    template<typename Range, typename Generator>
    void generate(Range& range, Generator generator)
    {
        return std::generate(begin(range), end(range), generator);
    }
}

namespace
{
    class RandomNumberBetween
    {
    public:
        RandomNumberBetween(int low, int high)
        : random_engine_{std::random_device{}()}
        , distribution_{low, high}
        {
        }
        int operator()()
        {
            return distribution_(random_engine_);
        }
    private:
        std::mt19937 random_engine_;
        std::uniform_int_distribution<int> distribution_;
    };

    void hashset_add_test(HashSet &mySet, std::unordered_set<int> &testSet, int nofItems)
    {
      auto scoped_span = trace::Scope(get_tracer()->StartSpan("hashset::test::add"));
      log("hashset::test::add");
      
      std::vector<int> numbersToAdd;
      std::generate_n(std::back_inserter(numbersToAdd), nofItems, RandomNumberBetween(1, 100));
      
      for (auto i = 0; i < nofItems; i++)
      {
        auto [it, expectedInserted] = testSet.insert(numbersToAdd[i]);
        auto actualInserted = mySet.add(numbersToAdd[i]);
        
        assert(expectedInserted == actualInserted);
      }
    }

    void hashset_remove_test(HashSet &mySet, std::unordered_set<int> &testSet, int nofItems)
    {
      auto scoped_span = trace::Scope(get_tracer()->StartSpan("hashset::test::remove"));
      log("hashset::test::remove");
      
      std::vector<int> numbersToRemove;
      std::generate_n(std::back_inserter(numbersToRemove), nofItems, RandomNumberBetween(50, 100));
      
      for (auto i = 0; i < nofItems; i++)
      {
        auto expectedRemoved = testSet.erase(numbersToRemove[i]) != 0;
        auto actualRemoved = mySet.remove(numbersToRemove[i]);
        
        assert(expectedRemoved == actualRemoved);
      }
    }

    void hashset_contains_test(HashSet &mySet, std::unordered_set<int> &testSet, int nofItems)
    {
      auto scoped_span = trace::Scope(get_tracer()->StartSpan("hashset::test::contains"));
      log("hashset::test::contains");
      
      std::vector<int> numbersToCheck;
      std::generate_n(std::back_inserter(numbersToCheck), nofItems, RandomNumberBetween(1, 50));
      
      for (auto i = 0; i < nofItems; i++)
      {    
        auto expectedContains = testSet.find(numbersToCheck[i]) != testSet.end();
        auto actualContains = mySet.contains(numbersToCheck[i]);
        
        assert(expectedContains == actualContains);
      }
    }
}

void hashset_test()
{  
  auto scoped_span = trace::Scope(get_tracer()->StartSpan("hashset::test"));
  log("hashset::test");
    
  HashSet mySet;
  std::unordered_set<int> testSet;
  auto nofItems = 100;
  
  hashset_add_test(mySet, testSet, nofItems);
  hashset_remove_test(mySet, testSet, nofItems);
  hashset_contains_test(mySet, testSet, nofItems);
}
