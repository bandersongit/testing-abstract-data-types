# functorized-test-demo


[![CircleCI](https://circleci.com/gh/yourgithubhandle/functorized-test-demo/tree/master.svg?style=svg)](https://circleci.com/gh/yourgithubhandle/functorized-test-demo/tree/master)


**Contains the following libraries and executables:**

```
functorized-test-demo@0.0.0
│
├─test/
│   name:    TestFunctorizedTestDemo.exe
│   main:    TestFunctorizedTestDemo
│   require: functorized-test-demo.lib
│
├─library/
│   library name: functorized-test-demo.lib
│   namespace:    FunctorizedTestDemo
│   require:
│
└─executable/
    name:    FunctorizedTestDemoApp.exe
    main:    FunctorizedTestDemoApp
    require: functorized-test-demo.lib
```

## Developing:

```
npm install -g esy
git clone <this-repo>
esy install
esy build
```

## Running Binary:

After building the project, you can run the main binary that is produced.

```
esy x FunctorizedTestDemoApp.exe 
```

## Running Tests:

```
# Runs the "test" command in `package.json`.
esy test
```
