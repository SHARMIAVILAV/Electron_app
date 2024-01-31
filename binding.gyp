
{
  "targets": [
    {
      "target_name": "opencvAddon",
      "sources": ["cpp/opencvAddon.cpp",
      "cpp/firstCpp.cpp"],
      "cflags!": ["-fno-exceptions"],
      "cflags_cc!": ["-fno-exceptions"],
      "include_dirs": [
            "<!@(node -p \"require('node-addon-api').include\")",
            ".",
            "/usr/local/lib",
            "/usr/local/include/opencv4",
      ],
        'link_settings': {
            'libraries': [
                '-L/user/local/lib', 
                '-lopencv_core', 
                '-lopencv_imgproc', 
                '-lopencv_highgui',
                '-lopencv_imgcodecs',
            ],
        },
        # 'libraries': [],
        'dependencies': [
            "<!(node -p \"require('node-addon-api').gyp\")"
        ],
        'defines': [ 'NAPI_DISABLE_CPP_EXCEPTIONS' ],
        
      "conditions": [
        ["OS=='mac'", {
          "xcode_settings": {
            "GCC_ENABLE_CPP_EXCEPTIONS": "NO",
            "CLANG_CXX_LIBRARY": "libc++"
          }
        }],
        ["OS=='linux'", {
          "cflags": ["-fno-exceptions"]
        }],
        ["OS=='win'", {
          "cflags_cc": ["/EHs-"],
          "msvs_settings": {
            "VCCLCompilerTool": {
              "ExceptionHandling": 0
            }
          }
        }]
      ]
    }
  ]
}

# {
#     "targets": [{
#         "target_name": "opencvAddon",
#         "cflags!": [ "-fno-exceptions" ],
#         "cflags_cc!": [ "-fno-exceptions" ],
#         "sources": [
#             "cpp/opencvAddon.cpp"
#         ],
#         'include_dirs': [
#             "<!@(node -p \"require('node-addon-api').include\")"
#         ],
#         'libraries': [],
#         'dependencies': [
#             "<!(node -p \"require('node-addon-api').gyp\")"
#         ],
#         'defines': [ 'NAPI_DISABLE_CPP_EXCEPTIONS' ]
#     }]
# }