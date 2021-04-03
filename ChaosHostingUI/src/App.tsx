import React, { useState } from "react";
import { BrowserRouter as Router, Switch, Route, Link } from "react-router-dom";
import "./App.css";
import { Room } from "./features/room/Room";
import { Lobby } from "./features/room/Lobby";

function App() {
    const [friendText] = useState(() => {
        const friendValues: string[] = [
            "mates",
            "friends",
            "cunts",
            "buddies",
            "dudes",
            "bastards",
            "mean people",
            "uncouth individuals",
            "meanies",
            "acquaintances",
            "associates",
            "allies",
            "colleagues",
            "fuck knuckles",
            "organ donors",
            "meat sacks",
            "meat popsicles",
            "stoners",
        ];

        return friendValues[Math.floor(Math.random() * friendValues.length)];
    });

    return (
        <Router>
            <nav className="navbar navbar-expand-md navbar-dark bg-dark mb-4">
                <div className="container">
                    <Link to="/" className="navbar-brand">
                        To Lobby
                    </Link>
                </div>
            </nav>

            <div className="container">
                <h1 className="text-center">
                    GTA V Torture Tool{" "}
                    <small className="text-muted">
                        for use with '{friendText}'
                    </small>
                </h1>

                <Switch>
                    <Route path="/room/:roomCode">
                        <Room />
                    </Route>
                    <Route path="/">
                        <Lobby />
                    </Route>
                </Switch>
            </div>
        </Router>
    );
}

export default App;
