export type LedgerValve = {
  state: boolean;
  ts?: Date;
};

export type LedgerSoilSensor = {
  percentage: number;
  raw: number;
};

export type LedgerFlow = {
  rate: number;
  vol: number;
};

export type SmartIrrigationLedger = {
  valves?: LedgerValve[];
  soilSensors?: LedgerSoilSensor[];
  flow?: LedgerFlow;
};