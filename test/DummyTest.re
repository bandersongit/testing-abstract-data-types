open TestFramework;

describe("hello", ({test, _}) =>
  test("world", ({expect, _}) =>
    expect.bool(true).toBeFalse()
  )
);
