; ModuleID = 'source.spice'
source_filename = "source.spice"
target datalayout = "e-m:w-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-w64-windows-gnu"

%_s__char__Queue__charptr_long_long_long_long_int = type { i8*, i64, i64, i64, i64, i32 }

@0 = private unnamed_addr constant [24 x i8] c"Size: %d, Capacity: %d\0A\00", align 1

define i32 @main() local_unnamed_addr {
entry:
  %q1 = alloca %_s__char__Queue__charptr_long_long_long_long_int, align 8
  call void @_mp__Queue_char__ctor(%_s__char__Queue__charptr_long_long_long_long_int* nonnull %q1)
  call void @_mp__Queue_char__push__char(%_s__char__Queue__charptr_long_long_long_long_int* nonnull %q1, i8 72)
  call void @_mp__Queue_char__push__char(%_s__char__Queue__charptr_long_long_long_long_int* nonnull %q1, i8 101)
  call void @_mp__Queue_char__push__char(%_s__char__Queue__charptr_long_long_long_long_int* nonnull %q1, i8 108)
  call void @_mp__Queue_char__push__char(%_s__char__Queue__charptr_long_long_long_long_int* nonnull %q1, i8 108)
  call void @_mp__Queue_char__push__char(%_s__char__Queue__charptr_long_long_long_long_int* nonnull %q1, i8 111)
  call void @_mp__Queue_char__push__char(%_s__char__Queue__charptr_long_long_long_long_int* nonnull %q1, i8 33)
  %0 = call i64 @_mf__Queue_char__getSize(%_s__char__Queue__charptr_long_long_long_long_int* nonnull %q1)
  %1 = call i64 @_mf__Queue_char__getCapacity(%_s__char__Queue__charptr_long_long_long_long_int* nonnull %q1)
  %2 = call i32 (i8*, ...) @printf(i8* nonnull dereferenceable(1) getelementptr inbounds ([24 x i8], [24 x i8]* @0, i64 0, i64 0), i64 %0, i64 %1)
  %3 = call i1 @_mf__Queue_char__isEmpty(%_s__char__Queue__charptr_long_long_long_long_int* nonnull %q1)
  br i1 %3, label %cleanup, label %while

while:                                            ; preds = %entry, %while
  %4 = call i8 @_mf__Queue_char__pop(%_s__char__Queue__charptr_long_long_long_long_int* nonnull %q1)
  %5 = sext i8 %4 to i32
  %putchar = call i32 @putchar(i32 %5)
  %6 = call i1 @_mf__Queue_char__isEmpty(%_s__char__Queue__charptr_long_long_long_long_int* nonnull %q1)
  br i1 %6, label %cleanup, label %while

cleanup:                                          ; preds = %while, %entry
  call void @_mp__Queue_char__dtor(%_s__char__Queue__charptr_long_long_long_long_int* nonnull %q1)
  ret i32 0
}

declare void @_mp__Queue_char__ctor(%_s__char__Queue__charptr_long_long_long_long_int*) local_unnamed_addr

declare void @_mp__Queue_char__push__char(%_s__char__Queue__charptr_long_long_long_long_int*, i8) local_unnamed_addr

; Function Attrs: nofree nounwind
declare noundef i32 @printf(i8* nocapture noundef readonly, ...) local_unnamed_addr #0

declare i64 @_mf__Queue_char__getSize(%_s__char__Queue__charptr_long_long_long_long_int*) local_unnamed_addr

declare i64 @_mf__Queue_char__getCapacity(%_s__char__Queue__charptr_long_long_long_long_int*) local_unnamed_addr

declare i1 @_mf__Queue_char__isEmpty(%_s__char__Queue__charptr_long_long_long_long_int*) local_unnamed_addr

declare i8 @_mf__Queue_char__pop(%_s__char__Queue__charptr_long_long_long_long_int*) local_unnamed_addr

declare void @_mp__Queue_char__dtor(%_s__char__Queue__charptr_long_long_long_long_int*) local_unnamed_addr

; Function Attrs: nofree nounwind
declare noundef i32 @putchar(i32 noundef) local_unnamed_addr #0

attributes #0 = { nofree nounwind }
