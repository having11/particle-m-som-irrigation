export type Valve = {
  id: number;
  index: number;
  name: string;
  state: boolean;
  timestamp: Date;
  soil_value?: number;
};