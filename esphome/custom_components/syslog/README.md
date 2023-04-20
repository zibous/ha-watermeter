# esphome_syslog

A simple syslog component for esphome. The component is designed to auto attach itself to the logger core module (like the MQTT component does with the `log_topic`)

This component uses the https://github.com/arcao/Syslog library version 2.0 at its core

## How to

To install, locate your `esphome` folder, create a folder named `custom_components` got into it and execute 
```shell
git clone https://github.com/TheStaticTurtle/esphome_syslog.git syslog
```

Simply add this to the configuration of your esphome node: 
```yaml
syslog:
```

When used like this, the component will simply **broadcast its log to everyone on the network** to change this behavior you can add the `ip_address` and `port` option like this:
```yaml
syslog:
    ip_address: "192.168.1.53"
    port: 514
```

Default behavior strips the esphome color tags from the log (The `033[0;xxx` and the `#033[0m`) if you do not want this set the `strip_colors` option to `False`.

Default behavior also sets `enable_logger` to `True` if you wish to disable sending logger messages and only use the `syslog.log` action you can do so by setting it to `False`:

The action `syslog.log` has 3 settings:
```yaml
then:
    - syslog.log:
        level: 7
        tag: "custom_action"
        payload: "My log message"
```

Due to the differences in log levels of syslog and esphome I had to translate them, here is a table:
| Esphome level                  | Syslog level |
|--------------------------------|--------------|
| ESPHOME_LOG_LEVEL_NONE         | LOG_EMERG    |
| ESPHOME_LOG_LEVEL_ERROR        | LOG_ERR      |
| ESPHOME_LOG_LEVEL_WARN         | LOG_WARNING  |
| ESPHOME_LOG_LEVEL_INFO         | LOG_INFO     |
| ESPHOME_LOG_LEVEL_CONFIG       | LOG_NOTICE   |
| ESPHOME_LOG_LEVEL_DEBUG        | LOG_DEBUG    |
| ESPHOME_LOG_LEVEL_VERBOSE      | LOG_DEBUG    |
| ESPHOME_LOG_LEVEL_VERY_VERBOSE | LOG_DEBUG    |

This table is however open to discussion as it's my interpretation, if you want to change it you can do so in the `syslog_component.cpp` file and change the array at line 22

## Warning
This component should not break anything and should work with everything however if it doesn't please open an issue. 
I have successfully tested this component with an esp8266 and an esp32. BUT The esp32 seems to have issue when there is a lot of thing to send very fast which you can see turing boot when it prints the config, see [my comment in issue #7](https://github.com/TheStaticTurtle/esphome_syslog/issues/7#issuecomment-1236194816) for more details . 
