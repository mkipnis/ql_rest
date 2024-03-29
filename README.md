# ql_rest

`ql_rest` is an experimental RESTful interface for QuantLib. This project aims to simplify the development of microservices for risk management and pricing various of financial instruments in the distributed environment in real-time.

`ql_rest` package includes conversion classes between JSON and QuantLibAddin/C++, Python classes to simplify serialization of JSON Objects to QuantLibAddin/C++, and Examples.

[Python](https://github.com/mkipnis/ql_rest/blob/master/qlrest/python_package/src/ql_rest/vanillaswap.py)
```
class qlVanillaSwap(JSONEncoder):

    def __init__(self):
        
        self.ObjectId = "";
        self.PayerReceiver = "";
        self.Nominal = 0.0;
        self.FixSchedule = "";
        self.FixedRate = 0.0;
        self.FixDayCounter = "";
        self.FloatingLegSchedule = "";
        self.IborIndex = "";
        self.Spread = 0.0;
        self.FloatingLegDayCounter = "";
        self.PaymentConvention = "";
        self.Permanent = False;
        self.Trigger = False;
        self.Overwrite = False;

    def default(self, o):
        return o.__dict__
```

[C++](https://github.com/mkipnis/ql_rest/blob/master/qlrest/vanillaswap_reader.cpp)
```
std::string vanillaswap::qlVanillaSwap(ptree const& pt)
{
    return QuantLibAddinCpp::qlVanillaSwap(
              pt.get<std::string>("ObjectId"),
              pt.get<std::string>("PayerReceiver"),
              pt.get<double>("Nominal"),
              pt.get<std::string>("FixSchedule"),
              pt.get<double>("FixedRate"),
              pt.get<std::string>("FixDayCounter"),
              pt.get<std::string>("FloatingLegSchedule"),
              pt.get<std::string>("IborIndex"),
              pt.get<double>("Spread"),
              pt.get<std::string>("FloatingLegDayCounter"),
              pt.get<std::string>("PaymentConvention"),
              pt.get<bool>("Permanent"),
              pt.get<bool>("Trigger"),
              pt.get<bool>("Overwrite")
    );
}
```


## Dependencies

| Project| Version|
| ------ | ------ |
| [Boost/Beast](https://www.boost.org/doc/libs/1_80_0/libs/beast/doc/html/index.html)| 1.80 |
| [QuantLib](https://github.com/lballabio/QuantLib/releases) | 1.22 |
| [ObjectHandler/GenSrc/QuantLibAddinCpp](https://github.com/eehlers/QuantLibAddin-Old/releases/tag/QuantLibAddin-v1.22) | 1.22 |


## Basic Microservice 
[Basic Microservice](https://github.com/mkipnis/ql_rest/blob/master/qlrest/basic_microservice.hpp) is a set of helper classes designed to simplify the construction of basic micro-services. It includes a socket listener class, Quantlib pricing thread management class, and a producer-consumer queue to facilitate the communication between the socket session class and the pricing thread.


### Data Flow
* The client connects to a microservice, submits a request via HTTP post, and receives back a token from the server(microservice).
* The server pushes request to the producer-consumer queue for the pricing thread to process.
* After some time, client connects to the server to check the status of the request by providing a specified in step-one token.
* If a request is still in pending, the client must repeat the step number there, until the request is processed by the pricing thread.

![N|Solid](https://github.com/mkipnis/ql_rest/blob/master/misc/basic_micro_service.png?raw=true)



### Examples

#### US Treasuries - [https://ustreasuries.online](https://ustreasuries.online)
Python client posts market data and instrument term structures to two calculators.  Calculators build a discount curve from the provided market data, price bonds, and make results available for client to retrieve.  After 3 seconds, the client queries calculators by posting provided in step one token.

![N|Solid](https://github.com/mkipnis/ql_rest/blob/master/misc/example_ustreasuries.png?raw=true)

 
#### Libor Swaps [https://swap.ustreasuries.online](https://swap.ustreasuries.online)
ReactJS client publishes market data and term structures to a calculator and display results

![N|Solid](https://github.com/mkipnis/ql_rest/blob/master/misc/example_libor_swaps.png?raw=true)


#### Vanilla Options [https://options.ustreasuries.online](https://options.ustreasuries.online)
ReactJS front-end retrieves market data from the django server and posts results to a calculator through the same server that serves as a proxy.

![N|Solid](https://github.com/mkipnis/ql_rest/blob/master/misc/example_options.png?raw=true)

