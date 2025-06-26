include("/home/ltc/Program/Talkbox/Talkbox-client/build/.qt/QtDeploySupport.cmake")
include("${CMAKE_CURRENT_LIST_DIR}/Talkbox-client-plugins.cmake" OPTIONAL)
set(__QT_DEPLOY_I18N_CATALOGS "qtbase")

qt6_deploy_runtime_dependencies(
    EXECUTABLE /home/ltc/Program/Talkbox/Talkbox-client/build/Talkbox-client
    GENERATE_QT_CONF
)
