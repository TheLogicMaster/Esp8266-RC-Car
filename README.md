# ESP8266 RC Car

This project has some cool functionality, but it poorly does its main objective, the actual RC car part.
Because it uses individual HTTP requests for commanding the Car, there are issues if requests get missed.
Crashing might also be an issue.

## Install the dependencies
```bash
yarn
```

### Start the app in development mode (hot-code reloading, error reporting, etc.)
```bash
quasar dev
```

### Lint the files
```bash
yarn run lint
```

### Build the app for production
```bash
quasar build
```

### Customize the configuration
See [Configuring quasar.conf.js](https://quasar.dev/quasar-cli/quasar-conf-js).
