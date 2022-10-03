
import React , { useState, useEffect }   from "react";
import './Add.css';
import { Button} from 'react-bootstrap';
import 'bootstrap/dist/css/bootstrap.min.css';

import Amplify from "aws-amplify";
import { API } from 'aws-amplify';

import awsmobile from "./aws-exports";
Amplify.configure(awsmobile);

async function addRecipe(event) {                           //send the data to API
  event.preventDefault();
  const data = {
    body: {
      estimatedTime: formState.estimatedTime,
      name: formState.name,
      numSteps: formState.numSteps,
      rating: null,
      recipeID: null,
      steps: formState.steps,
      brewer: formState.brewer,
      species: formState.species,
      origin: formState.origin,
      roasts: formState.roasts
    }
  };

  fetch("https://zlofv343ec.execute-api.us-east-1.amazonaws.com/proxy_test/user?_id=624f408eb7fa82502f4ba02b", {
     
    // Adding method type
    method: "POST",
     
    // Adding body or contents to send
    body: JSON.stringify(data),
     
    // Adding headers to the request
    headers: {
        "Content-type": "application/json; charset=UTF-8"
    }
})
 
// Converting to JSON
.then(response => response.json())
 
// Displaying results to console
.then(json => console.log(json));
 alert("recipe sent");
}

const formState = {
  estimatedTime: 600,
  name: "",
  numSteps: 11,
  steps: [ { description: "Measure your brew ratio (1:17 coffee:water)", stepType: "action", targetValue: 60  }], 
  brewer: "",
  species: "",
  origin: "",
  roasts: ""
}      

function updateFormState(key, value) {                              //update form state
    formState[key] = value;
  }

function Add() {
  // handle action change
  const handleInputChange = (e, index) => {
    const { name, value } = e.target;
    const list = [...stepsList];
    list[index][name] = value;
    setInputList(list);
  };
  
  // handle click event of the Remove button
  const handleRemoveClick = index => {
    const list = [...stepsList];
    list.splice(index, 1);
    setInputList(list);
  };
  
  // handle click event of the Add button
  const handleAddClick = () => {
    setInputList([...stepsList, { stepType: "", description: "", targetValue: "" }]);
  };

  const [stepsList, setInputList] = useState([{ stepType: "", description: "", targetValue: null }]);

  return (
    <body>
      <form>
      <h3><b>Customize Your Recipe</b></h3>
      <div class="all">
      <label>Name
        <input type="text" placeholder="Name" onChange={e => updateFormState('name', e.target.value)}/>
      </label>
      <label># of steps
        <input type="text" placeholder="# of steps" onChange={e => updateFormState('numSteps', e.target.value)}/>
      </label>
      <label>Time
        <input type="text" placeholder="time" onChange={e => updateFormState('estimatedTime', e.target.value)}/>
      </label>
      </div>
    
      <div className='Dropdown'>
            <div class ="brewer">
              <h5><b>Brewer&nbsp;&nbsp;</b></h5>
              <select id="Brewer" onChange={e => updateFormState('brewer', e.target.value)}>
                <option value="V60">V60</option>
                <option value="AeroPress">AeroPress</option>
                <option value="Chemex">Chemex</option>
                <option value="Moka Pot">Moka Pot</option>
                <option value="French Press">French Press</option>
                <option value="Espresso">Espresso</option>
              </select>
            <br />
            </div>
            
            <div class ="species">
              <h5><b>Species&nbsp;&nbsp;</b></h5>
              <select id="Species" onChange={e => updateFormState('species', e.target.value)}>
                <option value="Arabica">Arabica</option>
                <option value="Robusta">Robusta</option>
                <option value="Liberica">Liberica</option>
                <option value="Excelsa">Excelsa</option>
              </select>
            <br />
            </div>
            
            <div class ="origins">
              <h5><b>Origins&nbsp;&nbsp;</b></h5>
              <select id="Origins" onChange={e => updateFormState('origin', e.target.value)}>
                <option value="Colombia">Colombia</option>
                <option value="Brazil">Brazil</option>
                <option value="Ecuador">Ecuador</option>
                <option value="Honduras">Honduras</option>
              </select>
            <br />
            </div>
        
              <h5><b>Roasts&nbsp;&nbsp;</b></h5>
              <select id="Roasts" onChange={e => updateFormState('roasts', e.target.value)}>
                <option value="Light">Light</option>
                <option value="Medium">Medium</option>
                <option value="Dark">Dark</option>
                <option value="Extra Dark">Extra Dark</option>
              </select>
            <br />
          </div>
          
          <div steps>
            <div class="header">Steps</div>
            {stepsList.map((x, i) => {
              return (
                <div className="box">
                  <input
                    className="ml10"
                    name="stepType"
                    size="45"
                    placeholder="action or time"
                    value={x.stepType}
                    onChange={e => handleInputChange(e, i)} />&nbsp;&nbsp;
                  <input
                    className="ml10"
                    name="description"
                    placeholder="Description or N/A "
                    size="45"
                    value={x.description}
                    onChange={e => handleInputChange(e, i)} />&nbsp;&nbsp;
                  <input
                    className="ml10"
                    name="targetvalue"
                    placeholder="Time value or Weight Value"
                    size="25"
                    value={x.targetValue}
                    onChange={e => handleInputChange(e, i)} />
                  <div className="btn-box">
                    {stepsList.length !== 1 && <button
                      className="mr10"
                      onClick={() => handleRemoveClick(i)}>Remove</button>}
                    {stepsList.length - 1 === i && <button onClick={handleAddClick}>Add</button>}
                  </div>
                </div>
              );
            })}
            <br />
          </div>
          <Button onClick={function (e) { addRecipe(e); formState.steps = stepsList; } }>Submit</Button>
    </form>
</body>
  );
}

export default Add;
