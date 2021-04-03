import { useState } from "react";
import { useHistory } from "react-router-dom";

export function Lobby() {
    const history = useHistory();

    const [roomCode, setRoomCode] = useState("");

    const goToRoom = () => {
        history.push(`/room/${roomCode}`);
    };

    return (
        <>
            <br />
            <br />
            <br />
            <br />
            <div className="input-group input-group-lg">
                <span className="input-group-text" id="inputGroup-sizing-lg">
                    Enter Room Code
                </span>
                <input
                    type="text"
                    className="form-control"
                    aria-label="Enter Room Code"
                    aria-describedby="enterRoomCode"
                    value={roomCode}
                    onChange={(evt) => {
                        setRoomCode(evt.target.value);
                    }}
                />
                <button className="btn btn-outline-primary" type="button" onClick={goToRoom}>
                    Begin Torture
                </button>
            </div>
        </>
    );
}
