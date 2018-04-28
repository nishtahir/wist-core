module.exports = grunt => {
  grunt.initConfig({
    concat: {
      options: {
        sourceMap: true
      },
      "js": {
        src: ["src/js/*.js"],
        dest: "dist/lib.concat.js"
      },
    },
    uglify: {
      my_target: {
        options: {
          sourceMap: false,
          mangle: false,
        },
        files: {
          "dist/lib.min.js": ["dist/lib.concat.js"],
          "dist/libwist.js": ["dist/libwist.js"]
        }
      }
    },
    exec: {
      build: "emmake make",
      antlr: "./gradlew generateGrammarSource"
    },
    watch: {
      cpp: {
        files: ["src/cpp/*.cpp", "src/cpp/*.h"],
        tasks: ["uglify"]
      },
      js: {
        files: ["src/js/*.js"],
        tasks: ["concat", "uglify"]
      }
    }
  })
  grunt.loadNpmTasks("grunt-contrib-watch")
  grunt.loadNpmTasks('grunt-contrib-concat')
  grunt.loadNpmTasks('grunt-contrib-uglify')
  grunt.loadNpmTasks("grunt-exec")

  grunt.registerTask("default", ["watch"])
  grunt.registerTask("antlr", ["exec:antlr"])
}