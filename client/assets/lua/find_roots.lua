function u(r_i, r_f, v_i, v_f, a_i, t, a_0, a_1)
      local u = {}

      local t2 = t*t
      u[1] = (a_0[1]+a_i[1])*t + 0.5*a_1[1]*t2 + v_i[1] - v_f[1]
      u[2] = (a_0[2]+a_i[2])*t + 0.5*a_1[2]*t2 + v_i[2] - v_f[2]
      u[3] = (a_0[3]+a_i[3])*t + 0.5*a_1[3]*t2 + v_i[3] - v_f[3]

      return u
end

function du_da0(r_i, r_f, v_i, v_f, a_i, t, a_0, a_1)
      local du = {}

      du[1] = t
      du[2] = t
      du[3] = t
      
      return du
end

function du_da1(r_i, r_f, v_i, v_f, a_i, t, a_0, a_1)
      local du = {}

      local t2 = t*t
      du[1] = 0.5*t2
      du[2] = 0.5*t2
      du[3] = 0.5*t2
      
      return du
end

function v(r_i, r_f, v_i, v_f, a_i, t, a_0, a_1)
      local v = {}

      local t2 = t*t
      local t3 = t*t*t
      v[1] = 0.5*(a_0[1]+a_i[1])*t2 + 0.1667*a_1[1]*t3 + v_i[1]*t + r_i[1] - r_f[1]
      v[2] = 0.5*(a_0[2]+a_i[2])*t2 + 0.1667*a_1[2]*t3 + v_i[2]*t + r_i[2] - r_f[2]
      v[3] = 0.5*(a_0[3]+a_i[3])*t2 + 0.1667*a_1[3]*t3 + v_i[3]*t + r_i[3] - r_f[3]

      return v
end

function dv_da0(r_i, r_f, v_i, v_f, a_i, t, a_0, a_1)
      local dv = {}

      local t2 = t*t
      dv[1] = 0.5*t2
      dv[2] = 0.5*t2
      dv[3] = 0.5*t2
      
      return dv
end

function dv_da1(r_i, r_f, v_i, v_f, a_i, t, a_0, a_1)
      local dv = {}

      local t3 = t*t*t
      dv[1] = 0.1667*t3
      dv[2] = 0.1667*t3
      dv[3] = 0.1667*t3
      
      return dv
end

function secantEquationsOfMotion(r_i, r_f, v_i, v_f, a_i, t, a0_curr, a1_curr)

	local u_curr = u(r_i, r_f, v_i, v_f, a_i, t, a0_curr, a1_curr)
	local v_curr = v(r_i, r_f, v_i, v_f, a_i, t, a0_curr, a1_curr)
	local du_da0_curr = du_da0(r_i, r_f, v_i, v_f, a_i, t, a0_curr, a1_curr)
	local du_da1_curr = du_da1(r_i, r_f, v_i, v_f, a_i, t, a0_curr, a1_curr)
	local dv_da0_curr = dv_da0(r_i, r_f, v_i, v_f, a_i, t, a0_curr, a1_curr)
	local dv_da1_curr = dv_da1(r_i, r_f, v_i, v_f, a_i, t, a0_curr, a1_curr)

	local denom = {}
	local numer_a0 = {}
	local numer_a1 = {}

	denom[1] = (du_da0_curr[1] * dv_da1_curr[1]) - (du_da1_curr[1] * dv_da0_curr[1])
	denom[2] = (du_da0_curr[2] * dv_da1_curr[2]) - (du_da1_curr[2] * dv_da0_curr[2])
	denom[3] = (du_da0_curr[3] * dv_da1_curr[3]) - (du_da1_curr[3] * dv_da0_curr[3])
	numer_a0[1] = (u_curr[1] * dv_da1_curr[1]) - (v_curr[1] * du_da1_curr[1])
	numer_a0[2] = (u_curr[2] * dv_da1_curr[2]) - (v_curr[2] * du_da1_curr[2])
	numer_a0[3] = (u_curr[3] * dv_da1_curr[3]) - (v_curr[3] * du_da1_curr[3])
	numer_a1[1] = (v_curr[1] * du_da0_curr[1]) - (u_curr[1] * dv_da0_curr[1])
	numer_a1[2] = (v_curr[2] * du_da0_curr[2]) - (u_curr[2] * dv_da0_curr[2])
	numer_a1[3] = (v_curr[3] * du_da0_curr[3]) - (u_curr[3] * dv_da0_curr[3])

	local a0_next = {}
	local a1_next = {}

	a0_next[1] = a0_curr[1] - (numer_a0[1]/denom[1])
	a0_next[2] = a0_curr[2] - (numer_a0[2]/denom[2])
	a0_next[3] = a0_curr[3] - (numer_a0[3]/denom[3])
	a1_next[1] = a1_curr[1] - (numer_a1[1]/denom[1])
	a1_next[2] = a1_curr[2] - (numer_a1[2]/denom[2])
	a1_next[3] = a1_curr[3] - (numer_a1[3]/denom[3])

	while (math.abs(a1_next[2] - a1_curr[2]) > .00001) do

		a0_curr[1] = a0_next[1]
		a0_curr[2] = a0_next[2]
		a0_curr[3] = a0_next[3]

		a1_curr[1] = a1_next[1]
		a1_curr[2] = a1_next[2]
		a1_curr[3] = a1_next[3]

		u_curr = u(r_i, r_f, v_i, v_f, a_i, t, a0_curr, a1_curr)
		v_curr = v(r_i, r_f, v_i, v_f, a_i, t, a0_curr, a1_curr)
		du_da0_curr = du_da0(r_i, r_f, v_i, v_f, a_i, t, a0_curr, a1_curr)
		du_da1_curr = du_da1(r_i, r_f, v_i, v_f, a_i, t, a0_curr, a1_curr)
		dv_da0_curr = dv_da0(r_i, r_f, v_i, v_f, a_i, t, a0_curr, a1_curr)
		dv_da1_curr = dv_da1(r_i, r_f, v_i, v_f, a_i, t, a0_curr, a1_curr)

		denom[1] = (du_da0_curr[1] * dv_da1_curr[1]) - (du_da1_curr[1] * dv_da0_curr[1])
		denom[2] = (du_da0_curr[2] * dv_da1_curr[2]) - (du_da1_curr[2] * dv_da0_curr[2])
		denom[3] = (du_da0_curr[3] * dv_da1_curr[3]) - (du_da1_curr[3] * dv_da0_curr[3])
		numer_a0[1] = (u_curr[1] * dv_da1_curr[1]) - (v_curr[1] * du_da1_curr[1])
		numer_a0[2] = (u_curr[2] * dv_da1_curr[2]) - (v_curr[2] * du_da1_curr[2])
		numer_a0[3] = (u_curr[3] * dv_da1_curr[3]) - (v_curr[3] * du_da1_curr[3])
		numer_a1[1] = (v_curr[1] * du_da0_curr[1]) - (u_curr[1] * dv_da0_curr[1])
		numer_a1[2] = (v_curr[2] * du_da0_curr[2]) - (u_curr[2] * dv_da0_curr[2])
		numer_a1[3] = (v_curr[3] * du_da0_curr[3]) - (u_curr[3] * dv_da0_curr[3])

		a0_next[1] = a0_curr[1] - (numer_a0[1]/denom[1])
		a0_next[2] = a0_curr[2] - (numer_a0[2]/denom[2])
		a0_next[3] = a0_curr[3] - (numer_a0[3]/denom[3])

		a1_next[1] = a1_curr[1] - (numer_a1[1]/denom[1])
		a1_next[2] = a1_curr[2] - (numer_a1[2]/denom[2])
		a1_next[3] = a1_curr[3] - (numer_a1[3]/denom[3])
	end

	return a0_next, a1_next
end

function solveEquationsOfMotion(r_i_x, r_i_y, r_i_z, r_f_x, r_f_y, r_f_z, v_i_x, v_i_y, v_i_z, v_f_x, v_f_y, v_f_z, a_i_x, a_i_y, a_i_z, t_str)
	r_i = {}
	r_i[1] = r_i_x
	r_i[2] = r_i_y
	r_i[3] = r_i_z

	r_f = {}
	r_f[1] = r_f_x
	r_f[2] = r_f_y
	r_f[3] = r_f_z

	v_i = {}
	v_i[1] = v_i_x
	v_i[2] = v_i_y
	v_i[3] = v_i_z

	v_f = {}
	v_f[1] = v_f_x
	v_f[2] = v_f_y
	v_f[3] = v_f_z

	a_i = {}
	a_i[1] = a_i_x
	a_i[2] = a_i_y
	a_i[3] = a_i_z

	a_min = {}
	a_min[1] = a_min_x
	a_min[2] = a_min_y
	a_min[3] = a_min_z

	a0_init = {}
	a0_init[1] = -.1
	a0_init[2] = 1
	a0_init[3] = -.1

	a1_init = {}
	a1_init[1] = .01
	a1_init[2] = .01
	a1_init[3] = .01

	t = t_str

	local a0_result = {}
	local a1_result = {}

   	a0_result, a1_result = secantEquationsOfMotion(r_i, r_f, v_i, v_f, a_i, t, a0_init, a1_init)
   	return a0_result[1], a0_result[2], a0_result[3], a1_result[1], a1_result[2], a1_result[3]
end

--call function with arguments written by game
a0x, a0y, a0z, a1x, a1y, a1z = solveEquationsOfMotion(p0x, p0y, p0z, p1x, p1y, p1z, v0x, v0y, v0z, v1x, v1y, v1z, gx, gy, gz, time)
