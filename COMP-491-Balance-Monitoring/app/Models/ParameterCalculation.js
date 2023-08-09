import { Matrix, EigenvalueDecomposition } from 'ml-matrix';
export function calculateDerivative(y, dxs) {
    var dydx = Array(y.length)

    // Forward FD for left end node
    dydx[0] = (-y[2] + 4*y[1] - 3*y[0]) / (dxs[0] + dxs[1])

    // Central FD for internal nodes
    var i
    for (i = 1; i < y.length - 1; i++) {
        dydx[i] = (y[i+1] - y[i-1]) / (dxs[i-1] + dxs[i])
    }

    // Backward FD for right end node
    dydx[dydx.length - 1] = -(-y[y.length - 3] + 4*y[y.length - 2] - 3*y[y.length - 1]) / (dxs[dxs.length - 1] + dxs[dxs.length - 2])
    console.log(dxs[dxs.length - 1])
    return dydx
}

export function calculateIntegral(y, dxs) {
    // Integrate using Simpson's 1/3 Rule
    /*
    var yI = y[0]

    for (i = 1; i < y.length - 1; i++) {
        yI += (i % 2 == 1 ? 4 * y[i] : 2 * y[i])
    }

    yI += y[y.length - 1]

    yI *= dx / 3
    return yI
    */
    // Trapezoid Method
    var integral = 0
    for (let i = 0; i < y.length - 1; i++) {
        integral += (y[i] + y[i+1]) * dxs[i] / 2 
    }
    return integral

}

export function rungeKuttaIntegral(y,dts) {
    // Initialize array
    const yI = new Array(y.length).fill(0)
    /*
    for (let i = 1; i < y.length; i++) {
        const dt = dts[i-1];
        const k1 = y[i-1];
        const k2 = y[i-1] + 0.5 * k1 * dt;
        const k3 = y[i-1] + 0.5 * k2 *dt;
        const k4 = y[i-1] + k3 * dt;
        
        yI[i] = yI[i-1] + (1 / 6) * (k1 + 2*k2 + 3*k3 + k4) * dt;
    }
    */
    for (let i = 1; i < y.length; i++) {
        yI[i] = yI[i - 1] + (y[i - 1] + y[i]) * dts[i-1] / 2
        //yI[i] = y[i-1] + calculateIntegral(y.slice(i - 1,i + 1), dts.slice(i-1, i))
    }

    return yI
}

export function calculateLength(x, y) {
    var pathLength = 0;
    for (var i = 1; i < x.length; i++) {
      pathLength += Math.sqrt(
        (x[i] - x[i-1]) ** 2 +
        (y[i] - y[i-1]) ** 2
      )
    }
    return pathLength
}
/*
export function calculatePathLength(accX, accZ, dt) {
    // Integrate acceleration data to calculate velocity
    const velX = rungeKuttaIntegral(accX, dt)
    const velZ = rungeKuttaIntegral(accZ, dt)

    // Integrate again to calculate position
    const x = rungeKuttaIntegral(velX, dt)
    const z = rungeKuttaIntegral(velZ, dt)

    // Calculate path length
    const pathLength = calculateLength(x, z)

    return pathLength
} 
*/
export function calculatePathLength(x, y, dt) {
    return {
        pl: calculateLength(x, y),
        plx: x.reduce((a, b) => Math.abs(a) + Math.abs(b), 0),
        plz: y.reduce((a, b) => Math.abs(a) + Math.abs(b), 0)
    }
}


/*
export function calculateJerk(x, z, dt) {
    // Calculate derivative of x and y
    var jerkX = calculateDerivative(x, dt)
    var jerkZ =  calculateDerivative(z, dt)

    // Take square of jerk
    var jerkX2 = jerkX.map(a => a**2)
    var jerkZ2 = jerkZ.map(a => a**2)

    // Integrate jerk
    var jerkXI = calculateIntegral(jerkX2, dt)
    var jerkZI = calculateIntegral(jerkZ2, dt)

    return (1 / 2) * (jerkXI + jerkZI)

}
*/
export function addVectors(a, b) {
    const res = new Array(a.length).fill(0)

    for (let i = 0; i < res.length; i++) {
        res[i] = a[i] + b[i]
    }
    
    return res
}

export function calculateJerk(x, z, dt) {
    // Calculate derivative of x and y
    const dAccX = calculateDerivative(x, dt)
    const dAccZ =  calculateDerivative(z, dt)
    

    // Take square of jerk
    const dAccX2 = dAccX.map(a => a**2)
    const dAccZ2 = dAccZ.map(a => a**2)

    // Calculate jerk
    const jerk = (1 / 2) * calculateIntegral(addVectors(dAccX2, dAccZ2), dt)
    const jerkX = (1 / 2) * calculateIntegral(dAccX2, dt)
    const jerkZ = (1 / 2) * calculateIntegral(dAccZ2, dt)

    return {
        jerk: jerk,
        jerkX: jerkX,
        jerkZ: jerkZ
    }

}

export function calculateMeanVelocity(x, z, dt) {
    // Calculate Integral of Acceleration Data
    const velX = rungeKuttaIntegral(x, dt)
    const velZ = rungeKuttaIntegral(z, dt)

    // Calulcate mangitude of velocity
    const vel = new Array(velX.length)
    for (let i = 0; i < velX.length; i++) {
        vel[i] = Math.sqrt(velX[i]**2 + velZ[i]**2)
    }

    // Calculate mean velocity
    const meanVel = vel.reduce((a, b) => a + b, 0) / vel.length
    return {
        meanVel: meanVel,
        meanVelX: velX.reduce((a, b) => Math.abs(a) + Math.abs(b), 0) / velX.length,
        meanVelZ: velZ.reduce((a, b) => Math.abs(a) + Math.abs(b), 0) / velZ.length
    }

}


export function movingAverageFilter(x, w) {
    console.log("000")
    y = [];
    const hw = Math.floor(w/2)
    var avg = 0;
    console.log("111")
    for (let i = 0; i < hw; i++) {
      const sublist = x.slice(i, i+hw)
      avg = sublist.reduce((a, b) => a + b, 0) / sublist.length;
      y.push(avg)
    }
    console.log("222")
    
    for (let i = Math.floor(w/2); i < x.length - Math.floor(w/2); i++) {
      const sublist = x.slice(i - hw, i + hw + 1)
      avg = sublist.reduce((a, b) => a + b, 0) / sublist.length;
      y.push(avg)
    }
    console.log("333")
  
    for (let i = x.length - Math.floor(w/2); i < x.length; i++) {
      const sublist = x.slice(i - hw, i)
      avg = sublist.reduce((a, b) => a + b, 0) / sublist.length;
      y.push(avg)
    }
    console.log("444")
    return y
  }

  export function get95ellipse(x, y){

    var centerX = x.reduce((a, b) => a + b, 0)/ x.length;
    var centerY = y.reduce((a, b) => a + b, 0)/ y.length;

    var centeredX = x.map(num => num - centerX); 
    var centeredY = y.map(num => num - centerY); 

    var Cxx = centeredX.reduce((a, num) => a + (num ** 2), 0);
    var Cyy = centeredY.reduce((a, num) => a + (num ** 2), 0);
    var Cxy = 0;
    for(var i=0; i < centeredX.length; i++){
        Cxy += centeredX[i] * centeredY[i];
    }

    var cov_mat = [[Cxx, Cxy], [Cxy, Cyy]];
    console.log(cov_mat);

    var A = new Matrix(cov_mat);
    var e = new EigenvalueDecomposition(A);
    var real = e.realEigenvalues;
    var vectors = e.eigenvectorMatrix.to2DArray();

    var bigIndex = -1;
    var smallIndex = -1;

    if(real[0] > real[1]){
        bigIndex = 0;
        smallIndex = 1;
    } else {
        bigIndex = 1;
        smallIndex = 0;
    }

    var sig0 = Math.sqrt(real[bigIndex] / (centeredX.length - 1));
    var sig1 = Math.sqrt(real[smallIndex] / (centeredX.length - 1));

    var mainHalfAxis = [Math.sqrt(5.991) * sig0 * vectors[bigIndex][0], Math.sqrt(5.991) * sig0 * vectors[bigIndex][1]];
    var minorHalfAxis = [Math.sqrt(5.991) * sig1 * vectors[smallIndex][0], Math.sqrt(5.991) * sig1 * vectors[smallIndex][1]];
    
    var r1 = Math.sqrt(mainHalfAxis[0] ** 2 + mainHalfAxis[1] ** 2);
    var r2 = Math.sqrt(minorHalfAxis[0] ** 2 + minorHalfAxis[1] ** 2);

    var area = 5.991 * Math.PI * sig0 * sig1;
    console.log(area);

    var rotationAngle = Math.atan2(mainHalfAxis[1], mainHalfAxis[0]);

    const elx = [];
    const ely = [];
    counter = 0;
    for (var i = 0; i < 360; i += 0.1){
      var angle = i/180*Math.PI;
      elx[counter] = centerX + Math.cos(rotationAngle) * r1 * Math.cos(angle) - Math.sin(rotationAngle) * r2 * Math.sin(angle);
      ely[counter] = centerY + Math.sin(rotationAngle) * r1 * Math.cos(angle) + Math.cos(rotationAngle) * r2 * Math.sin(angle);
      counter++;
    }
    //console.log(elx);
    return [elx, ely, area];
  }