import React, { useState, useEffect }  from "react";
import './Home.css';

//import {Container, Button, Form} from 'react-bootstrap';
import Amplify from 'aws-amplify'      

//import { withAuthenticator } from '@aws-amplify/ui-react';

import awsmobile from "./aws-exports.js";
Amplify.configure( awsmobile);

function Home() {
  /*var user = {email: "123@gmail.com", recipesAdded: [1,2], numRecipesCompleted: 4, password: "dummypassword3",
            recipesCompleted: [1,1,2,2], userID: 3, username: "111"}; */
    
      const [user, setUser] = useState({});
      //const [favrecipes, setfavrecipes] = useState([]);
      //const [completedrecipes, setcompleted] = useState([]);

useEffect(() => {
  fetch('https://zlofv343ec.execute-api.us-east-1.amazonaws.com/proxy_test/user?_id=624f35fb01a99964a10916d5')
  .then(response => response.json())
  .then(data => {console.log(data); 
                 setUser(data);          
})
},[]);
console.log(user);

/*
useEffect(() => {
  fetch('https://zlofv343ec.execute-api.us-east-1.amazonaws.com/proxy_test/user?_id=624f35fb01a99964a10916d5')
  .then(response => response.json())
  .then(data => {console.log(data); 
                 console.log(typeof(favrecipes));
                 setfavrecipes(data.recipesAdded);
                 console.log(typeof(favrecipes));
                 
                })
},[favrecipes]);

useEffect(() => {
  fetch('https://zlofv343ec.execute-api.us-east-1.amazonaws.com/proxy_test/user?_id=624f35fb01a99964a10916d5')
  .then(response => response.json())
  .then(data => {console.log(data); 
                 console.log(typeof(favrecipes));
                 setcompleted(data.recipesCompleted);
                 console.log(typeof(favrecipes));
                 
                })
},[completedrecipes]);
*/


  return (                                    
    <><div class="User">
      <div class="d-flex flex-column text-center p-3 py-5">
        <span class="field">Name</span>
        <span class="value">{user.username}</span>               
        <span class="field">Email</span>
        <span class="value">{user.email}</span>
        <span class="field"># of Recipes Completed</span>
        <span class="value">{user.numRecipesCompleted}</span>
      </div>
    </div>
    
    <div class = "tables">
     <div class = "mostCommonRecipes">
      <h3 class ="header">Your Favorite Recipes</h3>
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
          {user.recipesAdded.map(r => (
                <tr>
                 <td style={{fontSize: '15px'}}>{r.name}</td>
                 <td style={{fontSize: '15px'}}>{r.numSteps}</td>
                 <td style={{fontSize: '15px'}}>{r.estimatedTime}</td>
                 <td style={{fontSize: '15px'}}>{r.rating}</td>
                 <td style={{fontSize: '15px'}}>{r.brewer}</td>
                 <td style={{fontSize: '15px'}}>{r.species}</td>
                 <td style={{fontSize: '15px'}}>{r.origin}</td>
                 <td style={{fontSize: '15px'}}>{r.roast}</td>
                </tr>
           ) )}
        </table>
       </div>
      
       <div class = "recipesCompleted">
        <h3 id="h" class ="header">Recipes Completed</h3>
          <table>
                 <tr>
                  <th style={{fontSize: '20px'}}>Name&nbsp;</th>
                  <th style={{fontSize: '20px'}}>Rating</th>
                 </tr>
          {user.recipesCompleted.map(r => (
                <tr>
                 <td style={{fontSize: '15px'}}>{r.name}</td>
                 <td style={{fontSize: '15px'}}>{r.rating}</td>
                </tr>
           ) )}
        </table>
       </div>
    </div>
  </>
    
); 
}

export default Home;

