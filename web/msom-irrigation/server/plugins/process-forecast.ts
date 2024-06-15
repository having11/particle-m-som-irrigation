import { NWSForecastOfficeId, gridpointForecast } from "~/integrations/weather";
import { getLatestSoilValues } from "../utils/db";
import { setValveState } from "../utils/device";

export default defineNitroPlugin(nitroApp => {
  setInterval(async () => {
    const { periods: forecast } = await gridpointForecast({
      wfo: <NWSForecastOfficeId>process.env.WEATHER_WFO,
      x: Number.parseInt(process.env.WEATHER_GRID_X ?? ''),
      y: Number.parseInt(process.env.WEATHER_GRID_Y ?? ''),
    });

    if (!forecast) {
      throw new Error('Null forecast!');
    }

    const nextPrecip = forecast[0].probabilityOfPrecipitation?.value;
    // Read latest soil % from db
    const values = await getLatestSoilValues();

    // Call watering algo
    if (nextPrecip && shouldWater(nextPrecip, values.map(v => v.value))) {
      for (var i = 0; i < 4; i++) {
        await setValveState(i, true);
      }
    }
  }, 5 * 60 * 1000);
});

export const shouldWater = (precipChance: number, soilValues: number[]): boolean => {
  // < 55% soil moisture
  const soilThreshold = 0.55;
  // >= 60% precip chance
  const precipThreshold = 0.6;

  // Don't water if rain is coming
  if (precipChance >= precipThreshold) {
    return false;
  }

  // Water if soil is too dry
  if (soilValues.reduce((prev, cur) => prev + cur) / soilValues.length < soilThreshold) {
    return true;
  }

  return false;
}