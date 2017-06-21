; ModuleID = 'test'

define i32 @foo(double, i32) {
entry:
  %y = alloca double
  %x = alloca double
  store double %0, double* %x
  store i32 %1, double* %y
  ret i32 19
}

define i32 @main() {
entry:
  %i = alloca i32
  store i32 10, i32* %i
  %d = alloca double
  store double 9.000000e-01, double* %d
  %a = alloca i32
  %b = alloca i32
  %c = alloca i32
  ret i32 10
}