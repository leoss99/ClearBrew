import React , { useState, useEffect } from "react";
import './Explore.css';

import { Button} from 'react-bootstrap';

import Amplify from "aws-amplify";
import { API } from 'aws-amplify';

import awsmobile from "./aws-exports";
Amplify.configure(awsmobile);


var user = {email: "123@gmail.com", mostCommonRecipes: [1,2], numRecipesCompleted: 4, password: "dummypassword3",
            recipesCompleted: [1,1,2,2], userID: 3, username: "111"}
var recipes = [
  { 
    "estimatedTime" : 600, 
    "name" : "Simple Arabica Pour-over", 
    "numSteps" : 11, 
    "rating" : 4.5, 
    "steps" : [ 
      { "description" : "Weigh 10 grams of coffee grounds", "stepType" : "weigh", "targetValue" : 10}, 
      { "description" : "Weigh 170 grams of water", "stepType" : "weigh", "targetValue" : 170}, 
      { "description" : "Boil the water", "stepType" : "action" , "targetValue" : 0}, 
      { "description" : "Grind the coffee", "stepType" : "action", "targetValue" : 0}, 
      { "description" : "Place and wet the filter", "stepType" : "action" , "targetValue" : 0}, 
      { "description" : "Place the coffee gorunds in the filter", "stepType" : "action", "targetValue" : 0}, 
      { "description" : "Pour 2 times as much water as coffee into the grounds", "stepType" : "action" , "targetValue" : 0}, 
      { "stepType" : "time", "targetValue" : 5 , "description" : "Pour the water"}, 
      { "description" : "Over the next 3 mins, continue to pour evenly over the grounds", "stepType" : "action" , "targetValue" : 0}, 
      { "stepType" : "time", "targetValue" : 180 , "description" :"Continue pouring"}, 
      { "description" : "Done!", "stepType" : "action" , "targetValue" : 0} 
    ],
    "brewer" : "Chemex",
    "origin": "Ecuador",
    "species" : "Arabica",
    "roast" : "Light",
    "user" : "624f3c650c2702f1c3ecaf1a"
  },
  ]


async function addRecipe(r) {                           //send the data to API
  const data = {
    body: {
      estimatedTime: r.estimatedTime,
      name: r.name,
      numSteps: r.numSteps,
      rating: r.rating,
      recipeID: r.recipeID,
      steps: r.steps,
      brewer: r.brewer,
      species: r.species,
      origin: r.origin,
      roasts: r.roasts
    }
  };
  console.log(data);
  //const response = await API.get('formapi', 'path + "/"+ currentUser' );   //how to get current user?
  //const user = await response.json();
  //const apiData = await API.post('formapi', 'path + "/" + user.id + "/" + "mostCommonRecipes"', JSON.stringify(data.body));       //?
}

function Explore() {


  return (
    <div class = "AllRecipes">
    <h3 class ="header">All Recipes</h3>
      <table>
               <tr>
                <th style={{fontSize: '20px'}}>Name&nbsp;&nbsp;&nbsp;</th>
                <th style={{fontSize: '20px'}}># of Steps&nbsp;&nbsp;&nbsp;</th>
                <th style={{fontSize: '20px'}}>Time&nbsp;&nbsp;&nbsp;</th>
                <th style={{fontSize: '20px'}}>Rating&nbsp;&nbsp;&nbsp;</th>
                <th style={{fontSize: '20px'}}>Brewer&nbsp;&nbsp;&nbsp;</th>
                <th style={{fontSize: '20px'}}>Species&nbsp;&nbsp;&nbsp;</th>
                <th style={{fontSize: '20px'}}>Origin&nbsp;&nbsp;&nbsp;</th>
                <th style={{fontSize: '20px'}}>Roasts</th>
               </tr>
        {recipes.map(r => (
              <tr>
               <td style={{fontSize: '15px'}}>{r.name}</td>
               <td style={{fontSize: '15px'}}>{r.numSteps}</td>
               <td style={{fontSize: '15px'}}>{r.estimatedTime}</td>
               <td style={{fontSize: '15px'}}>{r.rating}</td>
               <td style={{fontSize: '15px'}}>{r.brewer}</td>
               <td style={{fontSize: '15px'}}>{r.species}</td>
               <td style={{fontSize: '15px'}}>{r.origin}</td>
               <td style={{fontSize: '15px'}}>{r.roast}</td>
               <Button onClick={function(){addRecipe(r)}}>Add</Button>
              </tr>
         ) )}
      </table>
     </div>

  );
}

export default Explore;