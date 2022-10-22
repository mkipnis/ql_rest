/*  This file is part of ql_rest, a free-software/open-source library
    for utilization of QuantLib over REST */

import React from "react";


export const DayCounter = [

  { label: '30/360 (Bond Basis)', value:'30/360 (Bond Basis)'},
  { label: '30E/360 (Eurobond Basis)', value:'30E/360 (Eurobond Basis)'},
  { label: '30/360', value:'30/360'},
  { label: 'Actual/360', value:'Actual/360'},
  { label: 'Actual/365', value:'Actual/365'},
  { label: 'Actual/Actual', value:'Actual/Actual'},
  { label: 'Actual/Actual (ISDA)', value:'Actual/Actual (ISDA)'},
  { label: 'Bond Basis', value:'Bond Basis'},
  { label: 'Business252', value:'Business252'},

]


export const BusinessDateConventions = [

  { label: 'Following', value:'Following'},
  { label: 'Modified Following', value:'Modified Following'},
  { label: 'Modified Preceding', value:'Modified Preceding'},
  { label: 'Preceding',value:'Preceding'},
  { label: 'Unadjusted', value:'Unadjusted'},

]

export const Frequency = [

  { label: 'Annual', value:'Annual'},
  { label: 'Bimonthly', value:'Bimonthly'},
  { label: 'Biweekly', value:'Biweekly'},
  { label: 'Daily', value:'Daily'},
  { label: 'EveryFourthMonth', value:'EveryFourthMonth'},
  { label: 'EveryFourthWeek', value:'EveryFourthWeek'},
  { label: 'Monthly', value:'Monthly'},
  { label: 'Once', value:'Once'},
  { label: 'Quarterly', value:'Quarterly'},
  { label: 'Semiannual', value:'Semiannual'},
  { label: 'Weekly', value:'Weekly'},

]

export const DateGen = [
  { label: 'Forward', value:'Forward'},
  { label: 'ThirdWednesday', value:'ThirdWednesday'},
  { label: 'Twentieth', value:'Twentieth'},
  { label: 'TwentiethIMM', value:'TwentiethIMM'},
  { label: 'Zero', value:'Zero'},
  { label: 'Backward', value:'Backward'},
]

export const QuickSetupTenors = [
  {label:'1Y', value:'1Y'},
  {label:'2Y', value:'2Y'},
  {label:'3Y', value:'3Y'},
  {label:'5Y', value:'5Y'},
  {label:'7Y', value:'7Y'},
  {label:'10Y', value:'10Y'},
  {label:'15Y', value:'15Y'},
  {label:'20Y', value:'20Y'},
  {label:'25Y', value:'25Y'},
  {label:'30Y', value:'30Y'},
];

export const FixedFloat = [
  {label:'Fixed', value:'payer'},
  {label:'Float', value:'receiver'},
];
