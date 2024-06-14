const token: string = process.env.PARTICLE_API_TOKEN ?? '';
const deviceId: string = process.env.PARTICLE_IRRIGATION_LEDGER_DEVICE_ID ?? '';

export const setValveState = async (index: number, state: boolean): Promise<number> => {
  if (state) {
    return await valveHelper('activateValve', index);
  }
  else {
    return await valveHelper('shutoffValve', index);
  }
}

const valveHelper = async (funcName: string, index: number): Promise<number> => {
  const url = `https://api.particle.io/v1/devices/${deviceId}/${funcName}`;

  const responseData = await $fetch<{ return_value: number, }>(
    url, {
      method: 'POST',
      headers: [
        ['Authorization', `Bearer ${token}`],
        ['Accept', '*/*'],
        ['Content-Type', 'application/x-www-form-urlencoded; charset=UTF-8'],
      ],
      body: new URLSearchParams({
        arg: index.toString(),
      }),
    });

    return responseData.return_value;
}