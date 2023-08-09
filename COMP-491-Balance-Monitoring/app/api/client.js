// import { create } from "apisauce";
// // import cache from "../";
// import authStorage from "../auth/storage";
// import settings from "./settings";

// const apiClient = create({
//   baseURL: settings.apiUrl,
// });

// apiClient.addAsyncRequestTransform(async (request) => {
//     const authToken = await authStorage.getData();
//     if (!authToken) return;
//     request.headers["x-auth-token"] = authToken;
//   });
  
//   const get = apiClient.get;
//   apiClient.get = async (url, params, axiosConfig) => {
//     const response = await get(url, params, axiosConfig);
  
//     if (response.ok) {
//       cache.store(url, response.data);
//       return response;
//     }
  
//     const data = await cache.get(url);
//     return data ? { ok: true, data } : response;
//   };
  
  // export default apiClient;

  import axios from 'axios';

  axios.create({baseURL:'http://192.168.1.101:4000/api'});

  export default axios;