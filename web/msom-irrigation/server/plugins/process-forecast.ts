import { NWSForecastOfficeId, gridpointForecast } from "~/integrations/weather";

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
    // TODO: Read latest soil % from db

    // TODO: Call watering algo
  }, 5 * 60 * 1000);
});