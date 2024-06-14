import { gridpointForecast, NWSForecastOfficeId } from "~/integrations/weather";

export default defineEventHandler(async event => {
  return await gridpointForecast({
    wfo: <NWSForecastOfficeId>process.env.WEATHER_WFO,
    x: Number.parseInt(process.env.WEATHER_GRID_X ?? ''),
    y: Number.parseInt(process.env.WEATHER_GRID_Y ?? ''),
  });
});