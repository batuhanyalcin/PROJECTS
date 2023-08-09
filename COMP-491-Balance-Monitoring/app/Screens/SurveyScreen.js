import React, { Component } from 'react';
import { StyleSheet, Button, ScrollView, Text, TextInput, View, TouchableOpacity} from 'react-native';
import { SimpleSurvey } from 'react-native-simple-survey';
import { COLORS } from '../res/validColors.js';
import colors from '../config/colors';
import AppText from '../components/AppText';
import { LinearGradient } from 'expo-linear-gradient';
import Screen from '../components/Screen';
import SurveySelectButton from '../components/SurveySelectButton';


const GREEN = '#36c9bb';
const PURPLE = colors.gray;

const survey = [
    
    
  
    {
        questionType: 'SelectionGroup',
        questionText: '1. Does looking up increase your problem?',
        questionId: 'q1',
        options: [{optionText: 'Yes', value: 4}, {optionText: 'No', value: 0}, {optionText: 'Sometimes', value: 2}],
    },
    {
        questionType: 'SelectionGroup',
        questionText: '2. Because of your problem, do you feel frustrated?',
        questionId: 'q2',
        options: [{optionText: 'Yes', value: 4}, {optionText: 'No', value: 0}, {optionText: 'Sometimes', value: 2}],
    },
    {
        questionType: 'SelectionGroup',
        questionText: '3. Because of your problem, do you restrict your travel for business or recreation?' ,
        questionId: 'q3',
        options: [{optionText: 'Yes', value: 4}, {optionText: 'No', value: 0}, {optionText: 'Sometimes', value: 2}],
    },
    {
        questionType: 'SelectionGroup',
        questionText: '4. Does walking down the aisle of a supermarket increase your problem?' ,
        questionId: 'q4',
        options: [{optionText: 'Yes', value: 4}, {optionText: 'No', value: 0}, {optionText: 'Sometimes', value: 2}],
    },
    {
        questionType: 'SelectionGroup',
        questionText: '5. Because of your problem, do you have difficulty getting into or out of bed?' ,
        questionId: 'q5',
        options: [{optionText: 'Yes', value: 4}, {optionText: 'No', value: 0}, {optionText: 'Sometimes', value: 2}],
    },
    {
        questionType: 'SelectionGroup',
        questionText: '6. Does your problem significantly restrict your participation in social activities such as going out to dinner, to a movie, dancing, or to parties? ' ,
        questionId: 'q6',
        options: [{optionText: 'Yes', value: 4}, {optionText: 'No', value: 0}, {optionText: 'Sometimes', value: 2}],
    },
    {
        questionType: 'SelectionGroup',
        questionText: '7. Because of your problem, do you have difficulty reading?' ,
        questionId: 'q7',
        options: [{optionText: 'Yes', value: 4}, {optionText: 'No', value: 0}, {optionText: 'Sometimes', value: 2}],
    },
    {
        questionType: 'SelectionGroup',
        questionText: '8. Does performing more ambitious activities like sports, dancing, household chores such as sweeping or putting dishes away increase your problem?' ,
        questionId: 'q8',
        options: [{optionText: 'Yes', value: 4}, {optionText: 'No', value: 0}, {optionText: 'Sometimes', value: 2}],
    },
    {
        questionType: 'SelectionGroup',
        questionText: '9. Because of your problem, are you afraid to leave your home without having someone accompany you?' ,
        questionId: 'q9',
        options: [{optionText: 'Yes', value: 4}, {optionText: 'No', value: 0}, {optionText: 'Sometimes', value: 2}],
    },
    {
        questionType: 'SelectionGroup',
        questionText: '10. Because of your problem, have you been embarrassed in front of others? ' ,
        questionId: 'q10',
        options: [{optionText: 'Yes', value: 4}, {optionText: 'No', value: 0}, {optionText: 'Sometimes', value: 2}],
    },
    {
        questionType: 'SelectionGroup',
        questionText: '11. Do quick movements of your head increase your problem?' ,
        questionId: 'q11',
        options: [{optionText: 'Yes', value: 4}, {optionText: 'No', value: 0}, {optionText: 'Sometimes', value: 2}],
    },
    {
        questionType: 'SelectionGroup',
        questionText: '12. Because of your problem, do you avoid heights?' ,
        questionId: 'q12',
        options: [{optionText: 'Yes', value: 4}, {optionText: 'No', value: 0}, {optionText: 'Sometimes', value: 2}],
    },
    {
        questionType: 'SelectionGroup',
        questionText: '13. Does turning over in bed increase your problem?' ,
        questionId: 'q13',
        options: [{optionText: 'Yes', value: 4}, {optionText: 'No', value: 0}, {optionText: 'Sometimes', value: 2}],
    },
    {
        questionType: 'SelectionGroup',
        questionText: '14. Because of your problem, is it difficult for you to do strenuous housework or gardening?' ,
        questionId: 'q14',
        options: [{optionText: 'Yes', value: 4}, {optionText: 'No', value: 0}, {optionText: 'Sometimes', value: 2}],
    },
    {
        questionType: 'SelectionGroup',
        questionText: '15. Because of your problem, are you afraid people may think you are intoxicated?' ,
        questionId: 'q15',
        options: [{optionText: 'Yes', value: 4}, {optionText: 'No', value: 0}, {optionText: 'Sometimes', value: 2}],
    },
    {
        questionType: 'SelectionGroup',
        questionText: '16. Because of your problem, is it difficult for you to go for a walk by yourself?' ,
        questionId: 'q16',
        options: [{optionText: 'Yes', value: 4}, {optionText: 'No', value: 0}, {optionText: 'Sometimes', value: 2}],
    },
    {
        questionType: 'SelectionGroup',
        questionText: '17. Does walking along the pavement increase your problem?' ,
        questionId: 'q17',
        options: [{optionText: 'Yes', value: 4}, {optionText: 'No', value: 0}, {optionText: 'Sometimes', value: 2}],
    },
    {
        questionType: 'SelectionGroup',
        questionText: '18. Because of your problem, is it difficult for you to concentrate',
        questionId: 'q18',
        options: [{optionText: 'Yes', value: 4}, {optionText: 'No', value: 0}, {optionText: 'Sometimes', value: 2}],
    },
    {
        questionType: 'SelectionGroup',
        questionText: '19. Because of your problem, is it difficult for you to walk around the house in the dark?' ,
        questionId: 'q19',
        options: [{optionText: 'Yes', value: 4}, {optionText: 'No', value: 0}, {optionText: 'Sometimes', value: 2}],
    },
    {
        questionType: 'SelectionGroup',
        questionText: '20. Because of your problem, are you afraid to stay home alone?' ,
        questionId: 'q20',
        options: [{optionText: 'Yes', value: 4}, {optionText: 'No', value: 0}, {optionText: 'Sometimes', value: 2}],
    },
    {
        questionType: 'SelectionGroup',
        questionText: '21. Because of your problem, do you feel handicapped?' ,
        questionId: 'q21',
        options: [{optionText: 'Yes', value: 4}, {optionText: 'No', value: 0}, {optionText: 'Sometimes', value: 2}],
    },
    {
        questionType: 'SelectionGroup',
        questionText: '22. Has your problem placed stress on your relationships with members of your family or friends?' ,
        questionId: 'q22',
        options: [{optionText: 'Yes', value: 4}, {optionText: 'No', value: 0}, {optionText: 'Sometimes', value: 2}],
    },
    {
        questionType: 'SelectionGroup',
        questionText: '23. Because of your problem, are you depressed?' ,
        questionId: 'q23',
        options: [{optionText: 'Yes', value: 4}, {optionText: 'No', value: 0}, {optionText: 'Sometimes', value: 2}],
    },
    {
        questionType: 'SelectionGroup',
        questionText: '24. Does your problem interfere with your job or household responsibilities?' ,
        questionId: 'q24',
        options: [{optionText: 'Yes', value: 4}, {optionText: 'No', value: 0}, {optionText: 'Sometimes', value: 2}],
    },
    {
        questionType: 'SelectionGroup',
        questionText: '25. Does bending over increase your problem?' ,
        questionId: 'q25',
        options: [{optionText: 'Yes', value: 4}, {optionText: 'No', value: 0}, {optionText: 'Sometimes', value: 2}],
    },
    
   
    {
        questionType: 'Info',
        questionId: 'qq',
        questionText: 'Tap finish to see your results!',
        options:[{optionText: 'Finish', value: 0}]
    },

   
];

export default class SurveyScreen extends Component {
    
    constructor(props) {
        super(props);
        this.state = { backgroundColor: PURPLE, answersSoFar: '' };
    }

    onSurveyFinished(answers) {
        

        const infoQuestionsRemoved = [...answers];

        // Convert from an array to a proper object. This won't work if you have duplicate questionIds
        const answersAsObj = {};
        for (const elem of infoQuestionsRemoved) {
            console.log(elem.value) 
            answersAsObj[elem.questionId] = elem.value }
        
        
          
        console.log(answersAsObj);
        var result = 0;
        var emotion = 0;
        var func = 0;
        var phys = 0;
        for (var key in answersAsObj){
            console.log( key, answersAsObj[key] );
            result = result + answersAsObj[key]['value'];
            if(key === 'q1' || key === 'q4' || key === 'q8' || key === 'q11' || key === 'q13' || key === 'q17' || key === 'q25'){
                phys = phys + answersAsObj[key]['value'];
            }
            if(key === 'q3' || key === 'q5' || key === 'q6' || key === 'q7' || key === 'q12' || key === 'q14' || key === 'q16'|| key === 'q19'|| key === 'q24'){
                func = func + answersAsObj[key]['value'];
            }
            if(key === 'q2' || key === 'q9' || key === 'q10' || key === 'q15' || key === 'q18' || key === 'q20' || key === 'q21'|| key === 'q22'|| key === 'q23'){
                emotion = emotion + answersAsObj[key]['value'];
            }
        }
        
        console.log(result);
        console.log(emotion);
        console.log(phys);
        console.log(func);
        this.props.navigation.navigate('SurveyResultScreen', { result: result , emotion: emotion, phys: phys, func: func});
    }

    /**
     *  After each answer is submitted this function is called. Here you can take additional steps in response to the 
     *  user's answers. From updating a 'correct answers' counter to exiting out of an onboarding flow if the user is 
     *  is restricted (age, geo-fencing) from your app.
     */
    onAnswerSubmitted(answer) {
        this.setState({ answersSoFar: JSON.stringify(this.surveyRef.getAnswers(), 2) });
        switch (answer.questionId) {
            case 'favoriteColor': {
                if (COLORS.includes(answer.value.toLowerCase())) {
                    this.setState({ backgroundColor: answer.value.toLowerCase() });
                }
                break;
            }
            default:
                break;
        }
    }

    renderPreviousButton(onPress, enabled) {
        return (
            <View style={{ flexGrow: 1, maxWidth: 100, marginTop: 10, marginBottom: 10 }}>
                <Button
                    color={'#099773'}
                    onPress={onPress}
                    disabled={!enabled}
                    backgroundColor={GREEN}
                    title={'Previous'}
                />
            </View>
        );
    }

    renderNextButton(onPress, enabled) {
        return (
            <View style={{ flexGrow: 1, maxWidth: 100, marginTop: 10, marginBottom: 10 }}>
                <Button
                    color={'#099773'}
                    onPress={onPress}
                    disabled={!enabled}
                    backgroundColor={GREEN}
                    borderRadius = {10}
                    title={'Next'}
                />
            </View>
        );
    }

    renderFinishedButton(onPress, enabled) {
        return (
            <View style={{ flexGrow: 1, maxWidth: 100, marginTop: 10, marginBottom: 10 }}>
                <Button
                    title={'Finish'}
                    onPress={onPress}
                    disabled={!enabled}
                    color={'#099773'}
                />
            </View>
        );
    }

    renderButton(data, index, isSelected, onPress) {
        return (
            <View
                key={`selection_button_view_${index}`}
                style={{ marginTop: 5, marginBottom: 5, marginLeft:30,marginRight: 30,justifyContent: 'flex-start' }}
            >
                <SurveySelectButton title= {data.optionText}  style={styles.redirect} onPress={onPress} isSelected={isSelected}  />
                
            </View>
        );
    }

    renderQuestionText(questionText) {
        return (
                
                <View style = {styles.question}>
                <LinearGradient
                    colors={['rgb(145, 154, 255)', '#133a94']}
                    style={[styles.centered, {borderRadius:10}]}
                    useAngle={true}
                    angle={60}
                    start={{ x: 0, y: 0 }}
                    end={{ x: 1.5, y: 1 }}
                >
                <AppText style= {styles.text}>{questionText} </AppText>
                </LinearGradient>
                </View>
            
        );
    }

    renderInfoText(infoText) {
        return (
            <View style={{ marginLeft: 30, marginRight: 30 }}>
                <AppText style= {styles.infoText}>{infoText} </AppText>
            </View>
        );
    }

    render() {
        return (
            <Screen style={styles.container}>
               
                    <SimpleSurvey 
                    ref={(s) => { this.surveyRef = s; }}
                    survey={survey}
                    renderSelector={this.renderButton.bind(this)}
                    containerStyle={styles.surveyContainer}
                    selectionGroupContainerStyle={styles.selectionGroupContainer}
                    navButtonContainerStyle={{ flexDirection: 'row', justifyContent: 'space-around' }}
                    renderPrevious={this.renderPreviousButton.bind(this)}
                    renderNext={this.renderNextButton.bind(this)}
                    renderFinished={this.renderFinishedButton.bind(this)}
                    renderQuestionText={this.renderQuestionText}
                    onSurveyFinished={(answers) => this.onSurveyFinished(answers)}
                    onAnswerSubmitted={(answer) => this.onAnswerSubmitted(answer)}
                    renderTextInput={this.renderTextBox}
                    renderNumericInput={this.renderNumericInput}
                    renderInfo={this.renderInfoText}
                    />
            </Screen>
        );
    }
}

const styles = StyleSheet.create({
    container: {
        flex: 1,
        backgroundColor: colors.white,
        alignItems: 'center',
        justifyContent:'center',
        padding:10,
        //borderWidth:4,
        borderColor:colors.grey
    },
    question: {
        height: "40%",
        width: "90%",

        marginLeft: 35,
        marginRight: 30
        
        
      },
    surveyContainer: {
        width: 'auto',
        alignSelf: 'center',
        backgroundColor: colors.white,
        alignContent: 'center',
        
        
        padding: 5,
        flex: 0
    },
    selectionGroupContainer: {
        flexDirection: 'column',
        backgroundColor: colors.white,
        alignContent: 'flex-end',
        marginBottom: 30,
    },
    background: {
        flex: 1,
        minHeight: 800,
        maxHeight: 800,
       
    },
    
    infoText: {
        marginBottom: 20,
        fontSize: 20,
        marginLeft: 10,
        fontWeight: 800,
        textAlign: 'justify',
        color: 'rgb(145, 154, 255)'
        
    },
    text:{
        fontWeight: 800,
        fontSize: 20,
        textAlign: 'left',
        color:'white',
        marginLeft: 10,
        marginRight: 10
      },
      centered: {
        flex: 1 ,
        justifyContent: 'center'
      },
});
