import { getLatestValveStates } from "~/server/utils/db";

export default defineEventHandler(async event => {
  return await getLatestValveStates();
});