using System;
using System.Collections.Generic;
using System.Collections.Immutable;
using System.IO;
using System.Linq;
using System.Reflection;
using System.Text;
using System.Threading.Tasks;
using Microsoft.CodeAnalysis;
using Microsoft.CodeAnalysis.CSharp;
using Microsoft.CodeAnalysis.Text;
using Microsoft.CodeAnalysis.VisualBasic;

namespace GTA
{
    public class Compiler
    {
        private enum Language { CSharp, VisualBasic }

        private readonly IEnumerable<MetadataReference> references;


        public Compiler(IEnumerable<Assembly> assemblies)
        {
            references = from assembly in assemblies
                         select MetadataReference.CreateFromFile(assembly.Location);
        }

        /// <summary>
        /// Compile a file to an assembly.
        /// </summary>
        /// <param name="fileInfo">File to compile.</param>
        public Assembly CompileFile(FileInfo fileInfo)
        {
            Language language;

            switch (fileInfo.Extension)
            {
                case ".cs":
                    language = Language.CSharp;
                    break;
                case ".vb":
                    language = Language.VisualBasic;
                    break;
                default:
                    throw new NotSupportedException("Not supported extension");
            }

            using (Stream stream = fileInfo.OpenRead())
            {
                SourceText text = SourceText.From(stream);
                SyntaxTree tree; Compilation compilation = default(Compilation);



                switch (language)
                {
                    case Language.CSharp:
                        tree = CSharpSyntaxTree.ParseText(text);
                        compilation = CSharpCompilation.Create(fileInfo.Name, new[] { tree }, references, new CSharpCompilationOptions(OutputKind.DynamicallyLinkedLibrary));
                        break;
                    case Language.VisualBasic:
                        tree = VisualBasicSyntaxTree.ParseText(text);
                        compilation = VisualBasicCompilation.Create(fileInfo.Name, new[] { tree }, references, new VisualBasicCompilationOptions(OutputKind.DynamicallyLinkedLibrary));
                        break;
                }

                using (MemoryStream memoryStream = new MemoryStream())
                {
                    var result = compilation.Emit(memoryStream);
                    if (result.Success)
                        return Assembly.Load(memoryStream.GetBuffer());
                    else throw new CompilationException(result.Diagnostics);
                }
            }
        }

        [Serializable]
        public class CompilationException : Exception
        {
            private readonly string diagnostics;

            internal CompilationException(IEnumerable<Diagnostic> diagnostics)
            {
                this.diagnostics = diagnostics.Select(d => d.ToString()).Aggregate((a, r) => a + Environment.NewLine + r);
            }

            public override string Message
            {
                get
                {
                    return $"Compilation failed:{Environment.NewLine}{diagnostics}";
                }
            }
        }
    }
}
