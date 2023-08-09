import client from "./client";

const login = (email, password) => client.post("/user/auth/signin", { email, password });
const register = (userInfo) => client.post("/user/auth/signup", userInfo);

export default {
  login,
    register
};
