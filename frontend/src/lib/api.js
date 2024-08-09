import axios from 'axios';
import { PUBLIC_API } from "$env/static/public";

// Create an instance of Axios
const apiClient = axios.create({
  baseURL: `${PUBLIC_API}`, // The base URL for the API
  timeout: 10000,                     // Request timeout in milliseconds
});

export default apiClient;