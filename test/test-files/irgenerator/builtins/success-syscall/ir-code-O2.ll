; ModuleID = 'source.spice'
source_filename = "source.spice"

@anon.string.0 = private unnamed_addr constant [15 x i8] c"Hello syscall!\00", align 1

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @main() local_unnamed_addr #0 {
  %1 = tail call i64 @_Z12getRawLengthPKc(ptr nonnull @anon.string.0) #1
  tail call void asm sideeffect "movq $0, %rax\0Amovq $1, %rdi\0Amovq $2, %rsi\0Amovq $3, %rdx\0Amovq $4, %r10\0Amovq $5, %r8\0Amovq $6, %r9\0Asyscall\0A", "r,r,r,r,r,r,r,~{rax},~{rdi},~{rsi},~{rdx},~{r10},~{r8},~{r9},~{dirflag},~{fpsr},~{flags}"(i64 1, i64 1, i64 ptrtoint (ptr @anon.string.0 to i64), i64 %1, i64 0, i64 0, i64 0) #1
  ret i32 0
}

declare i64 @_Z12getRawLengthPKc(ptr) local_unnamed_addr

attributes #0 = { noinline nounwind optnone uwtable }
attributes #1 = { nounwind }
