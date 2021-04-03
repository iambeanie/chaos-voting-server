declare global {
    interface Window {
        settings: { webApiServer: string; useHTTPS: boolean };
    }
}

window.settings = window.settings || {};

export const createApiUrl = () => {
    let httpType: string = window.settings.useHTTPS ? "https" : "http";
    return `${httpType}://${window.settings.webApiServer}`;
};

export const apiUrl: string = `https://${window.settings.webApiServer}`;
export const hubUrl: string = `https://${window.settings.webApiServer}/votinghub`;
export const socketUrl: string = `wss://${window.settings.webApiServer}/Voting/RegisterVoterSocket`;
