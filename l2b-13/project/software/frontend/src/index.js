import React from "react";
import ReactDOM from "react-dom";
import "./index.css";
import { BrowserRouter as Router, Route, Routes } from "react-router-dom";
import {
  Navigation,
  Footer,
  Home,
  Add,
  Explore,
} from "./Components/ExportAll";

ReactDOM.render(
  <Router>
    <Navigation />
    <Routes>
      <Route path="/" element={<Home />} />
      <Route path="/Add" element={<Add />} />
      <Route path="/Explore" element={<Explore />} />
    </Routes>
    <Footer />
  </Router>,

  document.getElementById("root")
);

