from io import BytesIO
from gzip import GzipFile
from pathlib import Path
import logging
import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.config import strip_default_ids
from esphome.cpp_generator import ArrayInitializer
from esphome.yaml_util import dump, load_yaml
from esphome.core import CORE, coroutine_with_priority, ID
from esphome.components import web_server_base
from esphome.components.web_server_base import CONF_WEB_SERVER_BASE_ID
from esphome.const import (
    CONF_AUTH,
    CONF_FORCE_UPDATE,
    CONF_ID,
    CONF_PASSWORD,
    CONF_RAW_DATA_ID,
    CONF_USERNAME,
)

_LOGGER = logging.getLogger(__name__)

AUTO_LOAD = ["web_server_base", "web_server"]

CONF_BACKUP = "backup"

backup_ns = cg.esphome_ns.namespace("backup")
Backup = backup_ns.class_("Backup", cg.Component)

CONFIG_SCHEMA = cv.Schema(
    {
        cv.GenerateID(): cv.declare_id(Backup),
        cv.GenerateID(CONF_WEB_SERVER_BASE_ID): cv.use_id(
            web_server_base.WebServerBase
        ),
        cv.Optional(CONF_AUTH): cv.Schema(
            {
                cv.Required(CONF_USERNAME): cv.string_strict,
                cv.Required(CONF_PASSWORD): cv.string_strict,
            }
        ),
        cv.Optional(CONF_FORCE_UPDATE, default=False): cv.boolean,
    }
).extend(cv.COMPONENT_SCHEMA)


def _dump_config():
    return dump(strip_default_ids(load_yaml(CORE.config_path)))


@coroutine_with_priority(40.0)
async def to_code(config):
    paren = await cg.get_variable(config[CONF_WEB_SERVER_BASE_ID])

    var = cg.new_Pvariable(config[CONF_ID], paren)
    await cg.register_component(var, config)

    if CONF_AUTH in config:
        username = config[CONF_AUTH][CONF_USERNAME]
        if username:
            cg.add(var.set_username(username))
            password = config[CONF_AUTH][CONF_PASSWORD]
            if password:
                cg.add(var.set_password(password))

    gz_io = BytesIO()
    with GzipFile(
        fileobj=gz_io,
        mode="wb",
        compresslevel=9,
        mtime=None
        if config[CONF_FORCE_UPDATE]
        else Path(CORE.config_path).stat().st_mtime,
    ) as f:
        f.write(bytes(_dump_config(), encoding="utf8"))

    gz_config = gz_io.getvalue()

    arr = cg.progmem_array(
        ID(
            "backup_data_43c31a8938664f3a97597d916cb5c2ba",
            is_declaration=True,
            type=cg.uint8,
        ),
        ArrayInitializer(*[o for o in gz_config]),
    )
    cg.add(var.set_config(arr, len(gz_config)))

    _LOGGER.info(f"Backup config will take: {len(gz_config)} bytes")
