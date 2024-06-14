import { setValveState } from "~/server/utils/device";

// Returns true if successful
export default defineEventHandler(async event => {
  const data = await readBody<{index: number, state: boolean}>(event);

  return await setValveState(data.index, data.state);
});