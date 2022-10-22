
/*  This file is part of ql_rest, a free-software/open-source library
    for utilization of QuantLib over REST */
    
import React from "react";


export const SwapIndexes = [

  { Name: 'USD/Libor/3M-IsdaFixAm', Currency: 'USD', Index:'Libor', Tenor:'3M', FixingType:'IsdaFixAm' },
  { Name: 'USD/Libor/3M-IsdaFixPm', Currency: 'USD', Index:'Libor', Tenor:'3M', FixingType:'IsdaFixPm' },
  { Name: 'GBP/Libor/6M-Isda', Currency: 'GBP', Index:'Libor', Tenor:'6M', FixingType:'Isda' },
  { Name: 'CHF/Libor/6M-Isda', Currency: 'CHF', Index:'Libor', Tenor:'6M', FixingType:'Isda' },
  { Name: 'JPY/Libor/6M-IsdaFixAm', Currency: 'JPY', Index:'Libor', Tenor:'6M', FixingType:'IsdaFixAm' },
  { Name: 'JPY/Libor/6M-IsdaFixPm', Currency: 'JPY', Index:'Libor', Tenor:'6M', FixingType:'IsdaFixPm' },
  { Name: 'EUR/Libor/6M-IsdaFixA', Currency: 'EUR', Index:'Libor', Tenor:'6M', FixingType:'IsdaFixA' },
  { Name: 'EUR/Libor/6M-IsdaFixB', Currency: 'EUR', Index:'Libor', Tenor:'6M', FixingType:'IsdaFixB' },

]


export const SwapIndexTenors = [

    '1M','3M','6M', '9M', '1Y', '2Y', '3Y', '5Y', '7Y', '10Y', '15Y', '20Y', '30Y'

]
