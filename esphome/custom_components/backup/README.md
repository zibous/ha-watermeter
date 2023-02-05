# Backup

Save your ESPHome device configuration in firmware and and recover it if you lost source files. Just download it from `http://yourdevice.local/config.yaml`.

> WARNING: You should backup your all your files, this solution only for emergency purpose. I am not responsible for the loss or inability to recover data.

> WARNING: Stored configuration is the same as shown by `esphome config` command. It is fully worked but not the same as your original sources.

> WARNING: Command line substitutions, custom components and includes are not supported yet!

The configuration is very simple. Look at a sample below:
```yaml
# Full example of configuration entry
...
external_components:
  - source:
      type: git
      url: https://github.com/zdzichu6969/esphome_components
    components: [ backup ]
    refresh: 0s
...
backup:
  auth:
    username: !secret web_username
    password: !secret web_password
  force_update: false
```

##  Configuration variables:
* **auth** (*Optional*): Enables basic authentication with username and password.
 * **username** (**Required**, string): The username to use for authentication.
 * **password** (**Required**, string): The password to check for authentication.
* **force_update** (*Optional*, boolean): Unnecessary but might be needed when you changes in separated files are not detected properly. Default is False.
