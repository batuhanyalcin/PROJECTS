import ListItem from '../components/ListItem'

let patientID = 1;
let patientName = '';
let queryResult = [];
let accX = [];
let accZ = [];
let listArr = [];
let options = [];
let surveyQueryResult = [];
let overallScore = 0;
let functionalScore = 0;
let physicalScore = 0;
let emotionalScore = 0;
let surveyOptions = [];

export function setSurveyOptions(newSurveyOptions){
    surveyOptions = newSurveyOptions;
}

export function getSurveyOptions(){
    return surveyOptions;
}

export function setOverallScore(newOverallScore){
    overallScore = newOverallScore;
}

export function getOverallScore(){
    return overallScore;
}

export function setFunctionalScore(newFunctionalScore){
    functionalScore = newFunctionalScore;
}

export function getFunctionalScore(){
    return functionalScore;
}

export function setEmotionalScore(newEmotionalScore){
    emotionalScore = newEmotionalScore;
}

export function getEmotionalScore(){
    return emotionalScore;
}

export function setPhysicalScore(newPhysicalScore){
    physicalScore = newPhysicalScore;
}

export function getPhysicalScore(){
    return physicalScore;
}

export function setSurveyQueryResult(newSurveyQuery){
    surveyQueryResult = newSurveyQuery;
}
export function getSurveyQueryResult(){
    return surveyQueryResult;
}

export function setPatientID(newID) {
    patientID = newID;
}

export function getPatientID() {
  return patientID;
}

export function setPatientName(newName) {
    patientName = newName;
}

export function getPatientName() {
  return patientName;
}

export function setQueryResult(newResult) {
    queryResult = newResult;
}

export function getQueryResult() {
  return queryResult;
}

export function setAccX(newAccX) {
    accX = newAccX;
}

export function getAccX() {
  return accX;
}

export function setAccZ(newAccZ) {
    accZ = newAccZ;
}

export function getAccZ() {
  return accZ;
}

export function setListArr(queryResult, index){
    const listData = [
        {key: "Sway Area", value: queryResult[index].SwayArea},
        {key: "Path Length (m/s^2)", value: queryResult[index].PathLength},
        {key: "Path Length (Coronal) (m/s^2)", value: queryResult[index].PathLengthCor},
        {key: "Path Length (Sagittal) (m/s^2)", value: queryResult[index].PathLengthSag},
        {key: "Normalized Path Length (s^3/m)", value: queryResult[index].NormalizedPathLength},
        {key: "Jerk (m^2/s^5)", value: queryResult[index].Jerk},
        {key: "Jerk (m^2/s^5) (Coronal)", value: queryResult[index].JerkCor},
        {key: "Jerk (m^2/s^5) (Sagittal)", value: queryResult[index].JerkSag},
        {key: "Mean Velocity (m/s)", value: queryResult[index].MeanVel},
        {key: "Mean Velocity (m/s) (Coronal)", value: queryResult[index].MeanVelCor},
        {key: "Mean Velocity (m/s) (Sagittal)", value: queryResult[index].MeanVelSag}
    ]
    
    console.log(listData);

    listArr = [];
    for (let i = 0; i < listData.length; i++) {
    listArr.push(
        <ListItem key={i} text={listData[i].key} value={listData[i].value} />
    )
    }
}

export function getListArr(){
    return listArr;
}

export function getOptions(){
    return options;
}

export function setOptions(queryResult){
    options = [];

    for (let i = 0; i < queryResult.length; i++) {
        options.push(queryResult[i].TestDate);
        console.log(queryResult[i].TestDate);
    }
}