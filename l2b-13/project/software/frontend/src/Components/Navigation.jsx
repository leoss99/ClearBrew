import React from "react";
import { NavLink } from "react-router-dom";
import './Navigation.css'

function Navigation() {
  return (
  
    <div className="navigation">

        <nav className="navbar navbar-expand navbar-dark bg-dark">
          <div className="container">
          <img src="https://i.pinimg.com/564x/cf/71/93/cf7193d757d7231e6dc6834151356ccc.jpg" width="100" height="100" alt="ClearBrew"></img>
            <h2 color="white" style={{fontWeight: 'bold'}}> ClearBrew </h2>
            
            <div>
              <ul className="navbar-nav ml-auto">
                <li className="nav-item">
                  <NavLink className="nav-link" to="/">
                    Home&nbsp;
                    <span className="sr-only">(current)</span>
                  </NavLink>
                </li>
                <li className="nav-item">
                  <NavLink className="nav-link" to="/add">
                    Customize&nbsp;
                  </NavLink>
                </li>
                <li className="nav-item">
                  <NavLink className="nav-link" to="/Explore">
                    Explore
                  </NavLink>
                </li>
              </ul>
            </div>
          </div>
        </nav>
      </div>

  );
}

export default Navigation;