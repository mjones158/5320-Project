#ifndef MAX_HPP
#define MAX_HPP

#include <iostream>
#include <map>

#include <sam/AbstractConsumer.hpp>
#include <sam/BaseComputation.hpp>
#include <sam/ExponentialHistogram.hpp>
#include <sam/Features.hpp>
#include <sam/Util.hpp>
#include <sam/FeatureProducer.hpp>
#include <sam/tuples/Edge.hpp>
#include <vector>

namespace sam {

template <typename T, typename EdgeType, 
          size_t valueField, size_t... keyFields>
class Max: public AbstractConsumer<EdgeType>, 
                               public BaseComputation,
                               public FeatureProducer
{
private:

  // The size of the sliding window
  size_t N; 

  // A mapping from keyFields to the associated max. ******************************  <T>?
  std::map<std::string, std::shared_ptr<currentMax<T>>> allWindows;

  // store current max and value to be compared
  std::vector<int> valueStore;

  // the current max
  int max;


public:
  /**
   * Constructor.
   * \param N The size of the sliding window
   * \param nodeId The nodeId of the node that is running this operator.
   * \param featureMap The global featureMap that holds the features produced
   *                   by this operator.
   * \param identifier A unique identifier associated with this operator.
   */
  currentMax(size_t N, //int max
                          size_t nodeId,
                          std::shared_ptr<FeatureMap> featureMap,
                          std::string identifier) :
                          BaseComputation(nodeId, featureMap, identifier) 
                                          
  {
    
  }

  void setMax(int newMax)
  {
    max = newMax;
  }

  int getMax()
  {
    return max;
  }

   
  checkMax(size_t data)                                        
  {
    if(valuestore.empty() == true)
    {
      valueStore.pushback(data);
      setMax(valueStore[0]);
    }
    else
    {
      valuestore.pushback(data)
      if(valueStore[0] < valueStore[1])
      {
        valueStore.erase(begin);
        setMax(valueStore[0]);
      }
      else
      {
        valueStore.pop_back()
      }
    }
    return;
  }


  /**
   * Main method of an operator.  Processes the tuple.
   * \param input The tuple to process.
   */
  bool consume(EdgeType const& edge) 
  {
    this->feedCount++;

    if (this->feedCount % this->metricInterval == 0) {
      std::cout << "NodeId " << this->nodeId << " number of keys " 
                << allWindows.size() << " feedCount " << this->feedCount
                << std::endl;
    }

    // Generates unique key from key fields
    std::string key = generateKey<keyFields...>(edge.tuple);

    // Create an exponential histogram if it doesn't exist for the given key  **************************************
    if (allWindows.count(key) == 0) {
      auto eh = std::shared_ptr<ExponentialHistogram<T>>(
                  new ExponentialHistogram<T>(N, k));
      allWindows[key] = eh;
    }

    // auto eh = std::shared_ptr<currentMax<  ?  >>(
    //              new currentMax< ? >(values to be passed));
    //  allWindows[key] = eh;

    // Update the data structure **********************************************
    T value = std::get<valueField>(edge.tuple);
    allWindows[key]->add(value);

    // Getting the current max and providing that to the feature map.********************************************
    T currentMax = allWindows[key]->getTotal();  
    SingleFeature feature(currentMax);  

    // Update the freature map with the new feature.  The feature map
    // takes as input the key for this item, the identifier for this operator,
    // and the feature itself.  The key and the identifier together uniquely
    // identify the feature.
    this->featureMap->updateInsert(key, this->identifier, feature);

    this->notifySubscribers(edge.id, currentSum);

    return true;
  }

  void terminate() {}

};

}
#endif
