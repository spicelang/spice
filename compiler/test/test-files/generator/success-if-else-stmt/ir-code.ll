; ModuleID = 'test.spice'
source_filename = "test.spice"

@0 = private unnamed_addr constant [10 x i8] c"If branch\00", align 1
@1 = private unnamed_addr constant [10 x i8] c"Else if 1\00", align 1
@2 = private unnamed_addr constant [10 x i8] c"Else if 2\00", align 1
@3 = private unnamed_addr constant [5 x i8] c"Else\00", align 1

declare i32 @printf(i8*, ...)

declare i8* @malloc(i32)

declare void @free(i8*)

declare i8* @memcpy(i8*, i8*, i32)

define i32 @main() {
entry:
  %result = alloca i32, align 4
  store i32 0, i32* %result, align 4
  br i1 false, label %then, label %else

then:                                             ; preds = %entry
  %0 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([10 x i8], [10 x i8]* @0, i32 0, i32 0))
  br label %end6

else:                                             ; preds = %entry
  br i1 false, label %then1, label %else2

then1:                                            ; preds = %else
  %1 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([10 x i8], [10 x i8]* @1, i32 0, i32 0))
  br label %end5

else2:                                            ; preds = %else
  br i1 false, label %then3, label %else4

then3:                                            ; preds = %else2
  %2 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([10 x i8], [10 x i8]* @2, i32 0, i32 0))
  br label %end

else4:                                            ; preds = %else2
  %3 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @3, i32 0, i32 0))
  br label %end

end:                                              ; preds = %else4, %then3
  br label %end5

end5:                                             ; preds = %end, %then1
  br label %end6

end6:                                             ; preds = %end5, %then
  %4 = load i32, i32* %result, align 4
  ret i32 %4
}