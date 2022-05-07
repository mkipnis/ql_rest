import sys

sys.path.insert(0, '../../../qlrest/python_package/src/')

import ql_rest.bonds
import ql_rest.schedule
import datetime

class USTBond:

    def __init__(self):

        self.ust_bond_schedule = ql_rest.schedule.qlSchedule()

        self.ust_bond_schedule.Tenor = "6M"
        self.ust_bond_schedule.Convention = "Modified Following"
        self.ust_bond_schedule.TermDateConv = "Modified Following"
        self.ust_bond_schedule.Calendar = "US government bond market"
        self.ust_bond_schedule.GenRule = "Backward"

        self.ust_fixed_rate_bond = ql_rest.bonds.qlFixedRateBond()

        self.ust_fixed_rate_bond.Currency = "USD"
        self.ust_fixed_rate_bond.DayCounter = "Actual/Actual"
        self.ust_fixed_rate_bond.PaymentBDC = self.ust_bond_schedule.Convention
        self.ust_fixed_rate_bond.PaymentCalendar = self.ust_bond_schedule.Calendar
        self.ust_fixed_rate_bond.SettlementDays = 1
        self.ust_fixed_rate_bond.Redemption = 100
        self.ust_fixed_rate_bond.FaceAmount = 1000000


    def set_reference_data(self, ref_data):

        try:
            coupon = float(ref_data["interestRate"])

            self.ust_bond_schedule.ObjectId = "ust_bond_schedule/" + ref_data["cusip"]
            self.ust_bond_schedule.EffectiveDate = datetime.datetime.fromisoformat(ref_data["issueDate"]).strftime("%Y-%m-%d")
            self.ust_bond_schedule.TerminationDate = datetime.datetime.fromisoformat(ref_data["maturityDate"]).strftime("%Y-%m-%d")

            self.ust_fixed_rate_bond.ObjectId = "ust_fixed_rate_bond/" + ref_data["cusip"]

            self.ust_fixed_rate_bond.Coupons = [coupon/100.0]
            self.ust_fixed_rate_bond.ScheduleID = self.ust_bond_schedule.ObjectId
            self.ust_fixed_rate_bond.IssueDate = self.ust_bond_schedule.EffectiveDate

        except:
            return False

        return True

