<div align="center">
    <h3>API</h3>
</div>

---

### Table of Contents

- [Usage](#usage)
- [Get a valid access token](#get-a-valid-access-token)
- [API behavior](#api-behavior)
- [Learn more](#learn-more)

---

## Usage

The program uses Spotify's Web API to interact with the user's account and with Spotify's catalog.

The user uses an access token he obtained using the "authorization code flow".
During the process, the user has granted all rights necessary for the program to work correctly.

All data displayed by the program is obtained via the Spotify Web API and never stored after using the program.

All operations that are executed by the program can have an impact on the user's Spotify Account, as explicitly stated when the user grants requested rights.

## Get a valid access token

These steps will guide you to get a valid access token that can be used for the program to work correctly.

It is assumed that you already have a Spotify account, have cURL installed and know how to make cURL requests.

1. Create a [Spotify app](https://developer.spotify.com/documentation/web-api/tutorials/getting-started#create-an-app)
2. Go to the Dashboard of your newly created app and find its client id, its client secret and its redirect URI.
3. Using the values obtained during step 2, go the following url (replacing the italic values with real values:
    <pre>https://accounts.spotify.com/authorize?client_id=<em>client_id</em>&response_type=code&redirect_uri=<em>redirect_uri</em>&scope=user-library-read+user-library-modify+playlist-modify-public+playlist-modify-private+playlist-read-private+user-top-read+user-follow-read+user-follow-modify
    </pre>
4. Grant access and rights to the application on the Spotify page that is displayed (If you already did that step, the page won't be displayed and you can skip it).
5. After being redirected to the redirected uri you specified, a code should have been added to the uri, you should find it where you typed the URI in the following format:
    <pre><em>redirect_uri</em>?code=<em>code</em></pre>
    Save that code for the next step.
6. Make a cURL request in the following format, using values obtained in steps 2 and 5:
    <pre>
    curl -X POST "https://accounts.spotify.com/api/token" \
    -H "Content-Type: application/x-www-form-urlencoded" \
    -d "grant_type=authorization_code&code=<em>code</em>&redirect_uri=<em>redirect_uri</em>&client_id=<em>client_id</em>&client_secret=<em>client_secret</em>"
    </pre>
7. The API response should contain the access token.

You can use this access token for 1 hour before needing to refresh it. You can follow these steps again to get a new one or use the refresh token provided in the last response to generate another token, following one of the tutorials provided by the Spotify Web API docs.

## API behavior

The program works with API calls to the Spotify Web API and expects valid data to be returned from these calls.

Nevertheless, sometimes, for different reasons, the API can return errors that will make the program terminate. If the Spotify API is actually working, these errors can occur for mainly two reasons:

- The token you are using is invalid. You should be warned about it when starting the program, but it can sometimes occur that the token expires during program execution. To fix the error, you must refresh your token or get [another one](#get-a-valid-access-token).
- You've done too much requests recently. Considering how the program is built, this shouldn't occur, but if it does, you should wait a bit of time before using the program again (a few minutes should be sufficient).

## Learn more

You can learn more about the Spotify Web API by reading the [docs](https://developer.spotify.com/documentation/web-api).
