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
