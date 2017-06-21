
; ModuleID = 'test'

define i32 @foo(double, i32) {
entry:
  %y = alloca i32
  %x = alloca double
  store double %0, double* %x
  store i32 %1, i32* %y
  %x1 = load double* %x
  %gt_f = fcmp ogt double %x1, 1.000000e+00
  br i1 %gt_f, label %then, label %else

then:                                             ; preds = %entry
  %2 = load i32* %y
  ret i32 %2
  br label %ifcont

else:                                             ; preds = %entry
  ret i32 0
  br label %ifcont

ifcont:                                           ; preds = %else, %then
  unreachable
}

define i32 @main() {
entry:
  %i = alloca i32
  store i32 10, i32* %i
  %d = alloca double
  store double 5.000000e+00, double* %d
  %c = alloca i32
  store i32 1, i32* %c
  %t = alloca i1
  store i1 true, i1* %t
  %f = alloca i1
  store i1 false, i1* %f
  %i1 = load i32* %i
  %gt_i = icmp sgt i32 %i1, 1
  br i1 %gt_i, label %loop, label %continue

loop:                                             ; preds = %loop, %entry
  %i2 = load i32* %i
  %0 = sub i32 %i2, 1
  store i32 %0, i32* %i
  %i3 = load i32* %i
  %gt_i4 = icmp sgt i32 %i3, 1
  br i1 %gt_i4, label %loop, label %continue

continue:                                         ; preds = %loop, %entry
  %1 = load i32* %i
  ret i32 %1
  unreachable
}
