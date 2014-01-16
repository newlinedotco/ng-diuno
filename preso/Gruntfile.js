var fs = require('fs');

module.exports = function(grunt) {

  // Project configuration.
  grunt.initConfig({
    pkg: grunt.file.readJSON('package.json'),
    jshint: {
      files: ['src/js/reveal.js'],
      options: {}
    },
    copy: {
      app: {
        files: [{
          expand: true,
          cwd: 'src',
          src: ['js/**', 'css/**'],
          dest: 'dist/'
        }]
      }
    },
    template: {
      app: {
        options: {
          data: {
            title: 'Angular and Ardunio: powering human interaction',
            content: fs.readFileSync('src/slides.md').toString() 
          }
        },
        files: {
          'dist/index.html': ['src/layout.html']
        }
      }
    },
    connect: {
      dist: {
        options: {
        port: 5455,
        hostname: '0.0.0.0',
          middleware: function (connect) {
            return [
              require('grunt-contrib-livereload/lib/utils').livereloadSnippet,
              connect.static(require('path').resolve('dist'))
            ];
          }
        }
      }
    },
    open: {
      dist: {
        path: 'http://localhost:5455'
      }
    },
    clean: {
      dist: 'dist'
    },
    watch: {
      dist: {
        files: ['dist/**'],
        options: {
          livereload: true
        }
      },
      copy: {
        files: [
          'src/js/**',
          'src/css/**'
        ],
        tasks: ['copy']
      },
      template: {
        files: [
          'src/slides.md',
          'src/layout.html'
        ],
        tasks: ['template']
      }
    },
    mkcouchdb: {
      app: require('./couchapp.json')
    },
    couchapp: {
      app: require('./couchapp.json')
    }
  });

  // Load plugins
  require('matchdep').filterDev('grunt-*').forEach(grunt.loadNpmTasks);

  // Default task(s).
  grunt.registerTask('build', [
    'clean',
    'copy',
    'template'
  ]);

  grunt.registerTask('deploy', [
    'build',
    'mkcouchdb',
    'couchapp'
  ]);

  grunt.registerTask('server', [
    'build',
    'connect',
    'open',
    'watch'
  ]);

};