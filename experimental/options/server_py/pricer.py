# Copyright (c) Mike Kipnis

import QuantLib as ql

def american_option_with_greeks(
    calendar,
    today=ql.Date(),
    maturity=ql.Date(),
    spot=100.0,
    strike=100.0,
    rate=0.05,
    dividend=0.02,
    volatility=0.20,
    option_type='put'
):

    # Option type
    opt_type = ql.Option.Call if option_type.lower() == "call" else ql.Option.Put

    payoff = ql.PlainVanillaPayoff(opt_type, strike)
    exercise = ql.AmericanExercise(today, maturity)
    option = ql.VanillaOption(payoff, exercise)

    # Market data
    spot_handle = ql.QuoteHandle(ql.SimpleQuote(spot))

    risk_free_curve = ql.YieldTermStructureHandle(
        ql.FlatForward(today, rate, ql.Actual365Fixed())
    )

    dividend_curve = ql.YieldTermStructureHandle(
        ql.FlatForward(today, dividend, ql.Actual365Fixed())
    )

    # Use SimpleQuote so volatility can be bumped for numerical Greeks
    vol_quote = ql.SimpleQuote(volatility)

    vol_handle = ql.BlackVolTermStructureHandle(
        ql.BlackConstantVol(
            today,
            calendar,
            ql.QuoteHandle(vol_quote),
            ql.Actual365Fixed()
        )
    )

    process = ql.BlackScholesMertonProcess(
        spot_handle,
        dividend_curve,
        risk_free_curve,
        vol_handle
    )

    # Finite difference engine
    engine = ql.FdBlackScholesVanillaEngine(process)
    option.setPricingEngine(engine)

    def numerical_vega(option, vol_quote, bump=0.0001):
        original_vol = vol_quote.value()

        vol_quote.setValue(original_vol + bump)
        npv_up = option.NPV()

        vol_quote.setValue(original_vol - bump)
        npv_down = option.NPV()

        vol_quote.setValue(original_vol)

        return (npv_up - npv_down) / (2 * bump)

    def numerical_rho(option, today, rate, day_count, bump=0.0001):
        bumped_curve = ql.YieldTermStructureHandle(
            ql.FlatForward(today, rate + bump, day_count)
        )

        bumped_process = ql.BlackScholesMertonProcess(
            spot_handle,
            dividend_curve,
            bumped_curve,
            vol_handle
        )

        bumped_engine = ql.FdBlackScholesVanillaEngine(bumped_process)
        option.setPricingEngine(bumped_engine)

        npv_up = option.NPV()

        original_engine = ql.FdBlackScholesVanillaEngine(process)
        option.setPricingEngine(original_engine)

        npv_original = option.NPV()

        return (npv_up - npv_original) / bump

    results = {
        "price": round(option.NPV(),2),
        "delta": round(option.delta(),2),
        "gamma": round(option.gamma(),4),
        "vega": round(numerical_vega(option, vol_quote), 4),
        "theta": round(option.theta(), 4),
        "rho": round(numerical_rho(
            option,
            today,
            rate,
            ql.Actual365Fixed(), 4)
        )
    }

    return results


CALENDAR = ql.UnitedStates(ql.UnitedStates.NYSE)

def process_price_request(req: dict):

    price = req["price"]
    dividend_yield = req["dividend_yield"] / 100.0
    risk_free_rate = req["risk_free_rate"] / 100.0

    maturity = ql.DateParser.parseISO(req["exp_date"])
    today = ql.Date.todaysDate()

    ql.Settings.instance().evaluationDate = today

    results = {"calls": [], "puts": []}

    vols = req["vols"]

    for call in vols.get("calls", []):
        res = american_option_with_greeks(
            CALENDAR,
            today,
            maturity,
            price,
            call["strike"],
            risk_free_rate,
            dividend_yield,
            call["vol"] / 100.0,
            "call"
        )
        results["calls"].append({"strike": call["strike"], **res})

    for put in vols.get("puts", []):
        res = american_option_with_greeks(
            CALENDAR,
            today,
            maturity,
            price,
            put["strike"],
            risk_free_rate,
            dividend_yield,
            put["vol"] / 100.0,
            "put"
        )
        results["puts"].append({"strike": put["strike"], **res})

    return results
